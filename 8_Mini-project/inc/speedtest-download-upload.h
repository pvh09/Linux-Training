#ifndef _SPEED_DOWNLOAD_UPLOAD_H_
#define _SPEED_DOWNLOAD_UPLOAD_H_
#include "speedtest-cli.h"

float get_uptime(void);

void *calculate_dl_speed_thread();
void *download_thread(void *arg);
int speedtest_download(server_data_t *nearest_server);

void *calculate_ul_speed_thread();
void *upload_thread(void *arg);
int speedtest_upload(server_data_t *nearest_server);

#endif