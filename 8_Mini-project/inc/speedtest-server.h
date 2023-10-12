#ifndef _SPEED_SERVER_H_
#define _SPEED_SERVER_H_
#include "speedtest-config.h"

typedef struct client_data
{
    char ipAddr[MAX_SIZE];
    double latitude;
    double longitude;
    char isp[MAX_SIZE];
} client_data_t;

typedef struct server_data
{
    char url[MAX_SIZE];
    double latitude;
    double longitude;
    char name[MAX_SIZE];
    char country[MAX_SIZE];
    double distance;
    int latency;
    char domain_name[MAX_SIZE];
    struct sockaddr_in servinfo;
} server_data_t;

double calcDistance(double lat1, double lon1, double lat2, double lon2);

int get_ip_address_position(char *fileName, client_data_t *client_data);

int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers);

int get_best_server(server_data_t *nearest_servers);

void print_nearest_servers_table(server_data_t *nearest_servers);

#endif