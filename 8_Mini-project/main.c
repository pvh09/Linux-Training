#include "speedtest-cli.h"

extern int thread_all_stop;
void print_usage(const char *program_name)
{
    printf("Usage: %s [-t <num_threads>] [-p <protocol>] [-m]\n", program_name);
}

int parse_command_line(int argc, char **argv, int *num_threads, int *protocol, int *manual_mode)
{
    int opt;

    while ((opt = getopt(argc, argv, "t:p:m")) != -1)
    {
        switch (opt)
        {
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
    // default setting
    int num_threads = 3;
    int protocol = PROTOCOL_HTTP; // Default is HTTPS
    int manual_mode = 0;
    if (parse_command_line(argc, argv, &num_threads, &protocol, &manual_mode) != 0)
    {
        return 1;
    }

    printf("=========================================================\n");
    printf("Number of threads: %d\n", num_threads);
    printf("Protocol: %s\n", protocol == PROTOCOL_HTTP ? "HTTP" : "HTTPS");
    printf("Manual mode: %s\n", manual_mode ? "Enabled" : "Disabled");

    run_proc(num_threads, protocol);

    return 0;
}
