#include "speedtest-cli.h"

float start_dl_time, stop_dl_time, start_ul_time, stop_ul_time;
int thread_all_stop = 0;
int disable_real_time_reporting = 0, compute_dl_speed = 1, compute_ul_speed = 1;
long int total_dl_size = 0, total_ul_size = 0;

pthread_mutex_t pthread_mutex = PTHREAD_MUTEX_INITIALIZER;

thread_t thread[THREAD_NUMBER];

int get_ipv4_addr(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    // Find the first occurrence of ':' in the domain_name
    char *port_separator = strchr(domain_name, ':');

    if (port_separator != NULL)
    {
        // Cut the port part from the domain_name
        *port_separator = '\0';
    }

    if ((status = getaddrinfo(domain_name, "http", &hints, &addrinfo)) != 0)
    {
        fprintf(stderr, "Resolve DNS Failed: Can't get IP address for %s: %s\n", domain_name, gai_strerror(status));
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, p->ai_addr, sizeof(struct sockaddr_in));
            freeaddrinfo(addrinfo);
            return 1;
        }
    }

    fprintf(stderr, "Failed to find IPv4 address for %s\n", domain_name);
    freeaddrinfo(addrinfo);
    return 0;
}

int get_ipv4_https_addr(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char *token = strtok(domain_name, ":");

    if ((status = getaddrinfo(token, "https", &hints, &addrinfo)) != 0)
    {
        printf("Resolve DNS Failed: Can't get ip address! (%s)\n", token);
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, (struct sockaddr_in *)p->ai_addr, sizeof(struct sockaddr_in));
            break;
        }
    }

    freeaddrinfo(addrinfo);

    if (p == NULL)
    {
        printf("No IPv4 address found for %s\n", domain_name);
        return 0;
    }

    return 1;
}

int get_http_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[256] = {0}, tmp_path[128] = {0};
    char rbuf[8192];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;

    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        perror("Connect socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n\r\n",
            request_url, domain_name);

    if (send(fd, sbuf, strlen(sbuf), 0) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        if (fd)
            close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");
    if (fp == NULL)
    {
        perror("Error opening file for writing\n");
        close(fd);
        return 0;
    }

    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = recv(fd, rbuf, sizeof(rbuf), 0);

        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                fprintf(stderr, "Error receiving data from server\n");
                break;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, strlen(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }

    close(fd);
    fclose(fp);
    return 1;
}

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[256] = {0}, tmp_path[128] = {0};
    char rbuf[8192];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;

    SSL_CTX *ctx;
    SSL *ssl;

    SSL_library_init();
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        perror("SSL_CTX_new error!\n");
        return 0;
    }

    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) == -1)
    {
        perror("SSL_connect error!\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n"
            "Connection: close\r\n\r\n",
            request_url, domain_name);

    if (SSL_write(ssl, sbuf, strlen(sbuf)) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");

    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = SSL_read(ssl, rbuf, sizeof(rbuf));
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                printf("Can't get https file!\n");
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                close(fd);
                fclose(fp);
                return 0;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, strlen(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(fd);
    fclose(fp);
    return 1;
}

int get_ip_address_position(char *fileName, client_data_t *client_data)
{
    FILE *fp = NULL;
    char filePath[128] = {0}, line[512] = {0}, lat[64] = {0}, lon[64] = {0};
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

int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    char filePath[128] = {0}, line[512] = {0}, url[128] = {0}, lat[64] = {0}, lon[64] = {0}, name[128] = {0}, country[128] = {0};
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
    char latency_url[NEAREST_SERVERS_NUM][128], latency_request_url[128];
    char url[128], buf[128], filePath[64] = {0}, line[64] = {0};
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

void stop_all_thread(int signo)
{
    if (signo == SIGALRM)
    {
        thread_all_stop = 1;
    }
    return;
}

float get_uptime(void)
{
    FILE *fp;
    float uptime, idle_time;

    if ((fp = fopen("/proc/uptime", "r")) != NULL)
    {
        fscanf(fp, "%f %f\n", &uptime, &idle_time);
        fclose(fp);
        return uptime;
    }
    return -1;
}

void print_nearest_servers_table(server_data_t *nearest_servers)
{
    printf("=========================================================\n");
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
    printf("=========================================================\n");
}

void run_proc(int thread_num, int protocol)
{
    int i, best_server_index;
    client_data_t client_data;
    server_data_t nearest_servers[NEAREST_SERVERS_NUM];
    pthread_t pid;
    struct sockaddr_in servinfo;
    struct itimerval timerVal;

    memset(&client_data, 0, sizeof(client_data_t));
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        memset(&nearest_servers[i], 0, sizeof(server_data_t));
    }

    if (get_ipv4_https_addr(SPEEDTEST_DOMAIN_NAME, &servinfo))
    {
        if (!get_https_file(&servinfo, SPEEDTEST_DOMAIN_NAME, CONFIG_REQUEST_URL, CONFIG_REQUEST_URL))
        {
            printf("Can't get your IP address information.\n");
            return 0;
        }
    }
    if (get_ipv4_https_addr(SPEEDTEST_SERVERS_DOMAIN_NAME, &servinfo))
    {
        if (!get_https_file(&servinfo, SPEEDTEST_SERVERS_DOMAIN_NAME, SERVERS_LOCATION_REQUEST_URL, SERVERS_LOCATION_REQUEST_URL))
        {
            printf("Can't get servers list.\n");
            return 0;
        }
    }

    get_ip_address_position(CONFIG_REQUEST_URL, &client_data);
    printf("=========================================================\n");
    printf("Your IP Address : %s\n", client_data.ipAddr);
    printf("Your IP Location: %0.4lf, %0.4lf\n", client_data.latitude, client_data.longitude);
    printf("Your ISP        : %s\n", client_data.isp);
    printf("=========================================================\n");

    if (get_nearest_server(client_data.latitude, client_data.longitude, nearest_servers) == 0)
    {
        printf("Can't get server list.\n");
        return 0;
    }
    if ((best_server_index = get_best_server(nearest_servers)) != -1)
    {
        printf("\n==============The best server information================\n");
        printf("URL: %s\n", nearest_servers[best_server_index].url);
        printf("Latitude: %lf, Longitude: %lf\n", nearest_servers[best_server_index].latitude, nearest_servers[best_server_index].longitude);
        printf("Name: %s\n", nearest_servers[best_server_index].name);
        printf("Country: %s\n", nearest_servers[best_server_index].country);
        printf("Distance: %lf (km)\n", nearest_servers[best_server_index].distance);
        printf("Latency: %d (us)\n", nearest_servers[best_server_index].latency);

        print_nearest_servers_table(nearest_servers);

        signal(SIGALRM, stop_all_thread);
        timerVal.it_value.tv_sec = SPEEDTEST_DURATION;
        timerVal.it_value.tv_usec = 0;

        setitimer(ITIMER_REAL, &timerVal, NULL);
        pthread_create(&pid, NULL, calculate_dl_speed_thread, NULL);
        speedtest_download(&nearest_servers[1]);
        
        sleep(1);
        printf("\n");

        thread_all_stop = 0;
        setitimer(ITIMER_REAL, &timerVal, NULL);
        pthread_create(&pid, NULL, calculate_ul_speed_thread, NULL);
        speedtest_upload(&nearest_servers[1]);
        sleep(1);
        printf("\n");
    }
    return 0;
}
