#include "speedtest-server.h"

// Haversine formula
double calcDistance(double lat1, double lon1, double lat2, double lon2)
{
    int R = 6371; // Radius of the Earth
    double dlat, dlon, a, c, d;

    dlat = (lat2 - lat1) * M_PI / 180;
    dlon = (lon2 - lon1) * M_PI / 180;

    a = pow(sin(dlat / 2), 2) + cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) * pow(sin(dlon / 2), 2);
    c = 2 * atan2(sqrt(a), sqrt(1 - a));
    d = R * c;
    return d;
}

int get_ip_address_position(char *fileName, client_data_t *client_data)
{
    FILE *fp = NULL;
    char filePath[MAX_SIZE] = {0}, line[MAX_SIZE] = {0}, lat[MAX_SIZE] = {0}, lon[MAX_SIZE] = {0};
    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, fileName);

    if ((fp = fopen(filePath, "r")) != NULL)
    {
        while (fgets(line, sizeof(line) - 1, fp) != NULL)
        {
            if (!strncmp(line, "<client", 7))
            {
                // ex: <client ip="61.216.30.97" lat="24.7737" lon="120.9436" isp="HiNet" isprating="3.2" rating="0" ispdlavg="50329" ispulavg="22483" loggedin="0"/>
                sscanf(line, "%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"", client_data->ipAddr, lat, lon, client_data->isp);
                client_data->latitude = atof(lat);
                client_data->longitude = atof(lon);
                break;
            }
        }
        fclose(fp);
    }
    return 1;
}


int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    char filePath[MAX_SIZE] = {0}, line[MAX_SIZE] = {0}, url[MAX_SIZE] = {0}, lat[MAX_SIZE] = {0}, lon[MAX_SIZE] = {0}, name[MAX_SIZE] = {0}, country[MAX_SIZE] = {0};
    double lat_s, lon_s, distance;
    int count = 0, i = 0, j = 0;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, SERVERS_LOCATION_REQUEST_URL);

    if ((fp = fopen(filePath, "r")) != NULL)
    {
        while (fgets(line, sizeof(line) - 1, fp) != NULL)
        {
            if (!strncmp(line, "<server", 7))
            {
                // ex: <server url="http://88.84.191.230/speedtest/upload.php" lat="70.0733" lon="29.7497" name="Vadso" country="Norway" cc="NO" sponsor="Varanger KraftUtvikling AS" id="4600" host="88.84.191.230:8080"/>
                sscanf(line, "%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"", url, lat, lon, name, country);

                lat_s = atof(lat);
                lon_s = atof(lon);

                distance = calcDistance(lat_c, lon_c, lat_s, lon_s);

                for (i = 0; i < NEAREST_SERVERS_NUM; i++)
                {
                    if (nearest_servers[i].url[0] == '\0')
                    {
                        strncpy(nearest_servers[i].url, url, sizeof(url));
                        nearest_servers[i].latitude = lat_s;
                        nearest_servers[i].longitude = lon_s;
                        strncpy(nearest_servers[i].name, name, sizeof(name));
                        strncpy(nearest_servers[i].country, country, sizeof(country));
                        nearest_servers[i].distance = distance;
                        break;
                    }
                    else
                    {
                        if (distance <= nearest_servers[i].distance)
                        {
                            memset(&nearest_servers[NEAREST_SERVERS_NUM - 1], 0, sizeof(server_data_t));
                            for (j = NEAREST_SERVERS_NUM - 1; j > i; j--)
                            {
                                strncpy(nearest_servers[j].url, nearest_servers[j - 1].url, sizeof(nearest_servers[j - 1].url));
                                nearest_servers[j].latitude = nearest_servers[j - 1].latitude;
                                nearest_servers[j].longitude = nearest_servers[j - 1].longitude;
                                strncpy(nearest_servers[j].name, nearest_servers[j - 1].name, sizeof(nearest_servers[j - 1].name));
                                strncpy(nearest_servers[j].country, nearest_servers[j - 1].country, sizeof(nearest_servers[j - 1].country));
                                nearest_servers[j].distance = nearest_servers[j - 1].distance;
                            }
                            strncpy(nearest_servers[i].url, url, sizeof(url));
                            nearest_servers[i].latitude = lat_s;
                            nearest_servers[i].longitude = lon_s;
                            strncpy(nearest_servers[i].name, name, sizeof(name));
                            strncpy(nearest_servers[i].country, country, sizeof(country));
                            nearest_servers[i].distance = distance;
                            break;
                        }
                    }
                }
                count++;
            }
        }
    }
    if (count > 0)
        return 1;
    else
        return 0;
}

int get_best_server(server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    int i = 0, latency, best_index = -1;
    char latency_name[16] = "latency.txt";
    char latency_file_string[16] = "test=test";
    char latency_url[NEAREST_SERVERS_NUM][MAX_SIZE], latency_request_url[MAX_SIZE];
    char url[MAX_SIZE], buf[MAX_SIZE], filePath[MAX_SIZE] = {0}, line[MAX_SIZE] = {0};
    struct timeval tv1, tv2;
    struct sockaddr_in servinfo;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, latency_name);

    // Generate request url for latency
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        char *ptr = NULL;
        memset(latency_url[i], 0, sizeof(latency_url[i]));
        strncpy(url, nearest_servers[i].url, sizeof(nearest_servers[i].url));

        ptr = strtok(url, "/");
        while (ptr != NULL)
        {
            memset(buf, 0, sizeof(buf));
            strncpy(buf, ptr, strlen(ptr));

            // Change file name to "latency.txt"
            if (strstr(buf, "upload.") != NULL)
            {
                strcat(latency_url[i], latency_name);
            }
            else
            {
                strcat(latency_url[i], buf);
                strcat(latency_url[i], "/");
            }

            if (strstr(buf, "http:"))
                strcat(latency_url[i], "/");

            ptr = strtok(NULL, "/");
        }

        // Get domain name
        sscanf(latency_url[i], "http://%[^/]", nearest_servers[i].domain_name);

        // Get request url
        memset(latency_request_url, 0, sizeof(latency_request_url));
        if ((ptr = strstr(latency_url[i], nearest_servers[i].domain_name)) != NULL)
        {
            ptr += strlen(nearest_servers[i].domain_name);
            strncpy(latency_request_url, ptr, strlen(ptr));
        }

        if (get_ipv4_addr(nearest_servers[i].domain_name, &servinfo))
        {
            memcpy(&nearest_servers[i].servinfo, &servinfo, sizeof(struct sockaddr_in));

            // Get latency time
            gettimeofday(&tv1, NULL);
            get_http_file(&servinfo, nearest_servers[i].domain_name, latency_request_url, latency_name);
            gettimeofday(&tv2, NULL);
        }

        if ((fp = fopen(filePath, "r")) != NULL)
        {
            fgets(line, sizeof(line), fp);
            if (!strncmp(line, latency_file_string, strlen(latency_file_string)))
                nearest_servers[i].latency = (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;
            else
                nearest_servers[i].latency = -1;

            fclose(fp);
            unlink(filePath);
        }
        else
        {
            nearest_servers[i].latency = -1;
        }
    }

    // Select low latency server
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        if (i == 0)
        {
            best_index = i;
            latency = nearest_servers[i].latency;
        }
        else
        {
            if (nearest_servers[i].latency < latency && nearest_servers[i].latency != -1)
            {
                best_index = i;
                latency = nearest_servers[i].latency;
            }
        }
    }
    return best_index;
}

void print_nearest_servers_table(server_data_t *nearest_servers)
{
    printf("=====================================================================================================================================================================\n");
    // Print the table header
    printf("%-70s %-15s %-15s %-15s %-15s %-15s %-15s\n",
           "URL", "Latitude", "Longitude", "Name", "Country", "Distance (km)", "Latency (us)");
    printf("=====================================================================================================================================================================\n");
    for (int i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        printf("%-70s %-15.2f %-15.2f %-15s %-15s %-15.2f %-15i\n", nearest_servers[i].url,
               nearest_servers[i].latitude,
               nearest_servers[i].longitude,
               nearest_servers[i].name,
               nearest_servers[i].country,
               nearest_servers[i].distance,
               nearest_servers[i].latency);
    }
    printf("=====================================================================================================================================================================\n");
}