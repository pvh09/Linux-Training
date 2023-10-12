#ifndef _SPEED_UPLOAD_H_
#define _SPEED_UPLOAD_H_
#include "speedtest-download.h"

void *calculate_ul_speed_thread();
void *upload_thread(void *arg);
int speedtest_upload(server_data_t *nearest_server);

#endif