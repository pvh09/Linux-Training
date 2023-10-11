#include "speedtest-cli.h"

extern int thread_all_stop; 
void print_usage(const char *program_name) {
    printf("Usage: %s [-t <num_threads>] [-p <protocol>] [-m]\n", program_name);
}

int parse_command_line(int argc, char **argv, int *num_threads, int *protocol, int *manual_mode) {
    int opt;

    while ((opt = getopt(argc, argv, "t:p:m")) != -1) {
        switch (opt) {
            case 't':
                *num_threads = atoi(optarg);
                break;
            case 'p':
                if (strcmp(optarg, "http") == 0)
                    *protocol = PROTOCOL_HTTP;
                else if (strcmp(optarg, "https") == 0)
                    *protocol = PROTOCOL_HTTPS;
                break;
            case 'm':
                *manual_mode = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    int i, best_server_index;
    client_data_t client_data;
    server_data_t nearest_servers[NEAREST_SERVERS_NUM];
    pthread_t pid;
    struct sockaddr_in servinfo;
    struct itimerval timerVal;

    //default setting
    int num_threads = 3;
    int protocol = PROTOCOL_HTTPS; // Default is HTTPS
    int manual_mode = 0;

    if (parse_command_line(argc, argv, &num_threads, &protocol, &manual_mode) != 0) {
        return 1;
    }

    printf("=========================================================\n");
    printf("Number of threads: %d\n", num_threads);
    printf("Protocol: %s\n", protocol == PROTOCOL_HTTP ? "HTTP" : "HTTPS");
    printf("Manual mode: %s\n", manual_mode ? "Enabled" : "Disabled");

    //run_proc(num_threads, protocol);

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
        printf(" - thread_all_stop: %d\n", thread_all_stop);
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
