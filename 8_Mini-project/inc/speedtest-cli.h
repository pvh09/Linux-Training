#ifndef _SPEED_TEST_H_
#define _SPEED_TEST_H_
#include "speedtest-config.h"
#include "speedtest-server.h"
#include "speedtest-download.h"
#include "speedtest-upload.h"

void run_proc(int num_threads, int protocol);

#endif