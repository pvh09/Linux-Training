#ifndef _SPEED_CONFIG_H_
#define _SPEED_CONFIG_H_

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
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
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
#define PROTOCOL_HTTP 1
#define PROTOCOL_HTTPS 2
#define MAX_SIZE 256
typedef struct thread
{
    int thread_index;
    int running;
    pthread_t tid;
    char domain_name[MAX_SIZE];
    char request_url[MAX_SIZE];

    struct sockaddr_in servinfo;
} thread_t;

extern thread_t thread[THREAD_NUMBER];

float get_uptime(void);

int get_ipv4_addr(char *domain_name, struct sockaddr_in *servinfo);

int get_http_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename);

int get_ipv4_https_addr(char *domain_name, struct sockaddr_in *servinfo);

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename);

#endif