#ifndef _SPEED_TEST_H_
#define _SPEED_TEST_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

#define SPEEDTEST_DOMAIN_NAME "www.speedtest.net"
#define CONFIG_REQUEST_URL "speedtest-config.php"

#define SPEEDTEST_SERVERS_DOMAIN_NAME "c.speedtest.net"
#define SERVERS_LOCATION_REQUEST_URL "speedtest-servers-static.php?"

#define FILE_DIRECTORY_PATH "/tmp/"
#define NEAREST_SERVERS_NUM 3
#define THREAD_NUMBER 4
#define SPEEDTEST_DURATION 5

#define UL_BUFFER_SIZE 8192
#define UL_BUFFER_TIMES 10240
#define DL_BUFFER_SIZE 8192

typedef struct client_data
{
    char ipAddr[128];
    double latitude;
    double longitude;
    char isp[128];
} client_data_t;

typedef struct server_data
{
    char url[128];
    double latitude;
    double longitude;
    char name[128];
    char country[128];
    double distance;
    int latency;
    char domain_name[128];
    struct sockaddr_in servinfo;
} server_data_t;

typedef struct thread
{
    int thread_index;
    int running;
    pthread_t tid;
    char domain_name[128];
    char request_url[128];

    struct sockaddr_in servinfo;
} thread_t;

int get_ipv4_https_addr(char *domain_name, struct sockaddr_in *servinfo);

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename);

int get_ip_address_position(char *fileName, client_data_t *client_data);

int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers);

int get_best_server(server_data_t *nearest_servers);

void print_nearest_servers_table(server_data_t *nearest_servers);

void stop_all_thread(int signo);

void *calculate_dl_speed_thread();

int speedtest_download(server_data_t *nearest_server);

void *calculate_ul_speed_thread();

int speedtest_upload(server_data_t *nearest_server);

#endif