#ifndef _SPEED_DOWNLOAD_H_
#define _SPEED_DOWNLOAD_H_
#include "speedtest-cli.h"

void *calculate_dl_speed_thread();
void *download_thread(void *arg);
int speedtest_download(server_data_t *nearest_server);

#endif