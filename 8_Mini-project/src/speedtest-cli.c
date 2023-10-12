#include "speedtest-cli.h"

float start_dl_time, stop_dl_time, start_ul_time, stop_ul_time;
int thread_all_stop = 0;
int disable_real_time_reporting = 0, compute_dl_speed = 1, compute_ul_speed = 1;
long int total_dl_size = 0, total_ul_size = 0;

pthread_mutex_t pthread_mutex = PTHREAD_MUTEX_INITIALIZER;

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

    switch (protocol)
    {
    case PROTOCOL_HTTP:
        if (get_ipv4_addr(SPEEDTEST_DOMAIN_NAME, &servinfo))
        {
            if (!get_http_file(&servinfo, SPEEDTEST_DOMAIN_NAME, CONFIG_REQUEST_URL, CONFIG_REQUEST_URL))
            {
                printf("Can't get your IP address information.\n");
                return;
            }
        }
        if (get_ipv4_addr(SPEEDTEST_SERVERS_DOMAIN_NAME, &servinfo))
        {
            if (!get_http_file(&servinfo, SPEEDTEST_SERVERS_DOMAIN_NAME, SERVERS_LOCATION_REQUEST_URL, SERVERS_LOCATION_REQUEST_URL))
            {
                printf("Can't get servers list.\n");
                return;
            }
        }
        break;
    case PROTOCOL_HTTPS:
        if (get_ipv4_https_addr(SPEEDTEST_DOMAIN_NAME, &servinfo))
        {
            if (!get_https_file(&servinfo, SPEEDTEST_DOMAIN_NAME, CONFIG_REQUEST_URL, CONFIG_REQUEST_URL))
            {
                printf("Can't get your IP address information.\n");
                return;
            }
        }
        if (get_ipv4_https_addr(SPEEDTEST_SERVERS_DOMAIN_NAME, &servinfo))
        {
            if (!get_https_file(&servinfo, SPEEDTEST_SERVERS_DOMAIN_NAME, SERVERS_LOCATION_REQUEST_URL, SERVERS_LOCATION_REQUEST_URL))
            {
                printf("Can't get servers list.\n");
                return;
            }
        }
        break;
    default:
        printf("Invalid protocol specified.\n");
        return;
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
        return;
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
        pthread_create(&pid, NULL, calculate_dl_speed_thread, NULL);
        speedtest_download(&nearest_servers[1]);

        sleep(1);
        printf("\n");

        thread_all_stop = 0;
        pthread_create(&pid, NULL, calculate_ul_speed_thread, NULL);
        speedtest_upload(&nearest_servers[1]);
        sleep(1);
        printf("\n");
    }
}
