#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void timer_handler(int sig, siginfo_t *si, void *uc)
{
    printf("Timer expired!\n");
}

int main()
{
    struct sigevent sev;
    timer_t timerid;
    struct itimerspec its_terminate;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigaction(SIGALRM, &sa, NULL);
    int retry_count = 0;
    printf("Loading...\n");
    // Create a timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCK_REALTIME, &sev, &timerid);

    while (retry_count < 3)
    {
        its_terminate.it_value.tv_sec = 5;
        its_terminate.it_value.tv_nsec = 0;
        its_terminate.it_interval.tv_sec = 3;
        its_terminate.it_interval.tv_nsec = 0;
        timer_settime(timerid, 0, &its_terminate, NULL);

        while (1)
        {
            // Get the current timer value
            timer_gettime(timerid, &its_terminate);
            time_t remaining_seconds = its_terminate.it_value.tv_sec;

            if (remaining_seconds > 0)
            {
                printf("Remaining time before expiration: %ld seconds\n", remaining_seconds);
            }
            else
            {
                printf("Timer has expired!\n");
                break;
            }
            sleep(1);
        }
        retry_count++;
        if (retry_count < 3)
        {
            printf("Retrying...\n");
            sleep(1); // Wait before retrying
        }
    }

    if (timer_delete(timerid) == -1)
    {
        perror("timer_delete");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Timer %ld deleted successfully.\n", (long)timerid);
    }

    return 0;
}
