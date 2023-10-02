#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h> 

// Signal handler function
void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1 signal from the user.\n");
    } else if (signum == SIGUSR2) {
        printf("Received SIGUSR2 signal from the user.\n");
    }
}

int main() {
    // Register signal handlers for SIGUSR1 and SIGUSR2
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    // Set up a timer to send signals at specific times
    struct sigevent sev;
    timer_t timerid;
    struct itimerspec its;

    // Create the timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1; // Signal to be sent
    timer_create(CLOCK_REALTIME, &sev, &timerid);

    // Set the timer to trigger every 5 seconds
    its.it_value.tv_sec = 5;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 5;
    its.it_interval.tv_nsec = 0;

    // Start the timer
    timer_settime(timerid, 0, &its, NULL);

    printf("Sending SIGUSR1 signal every 5 seconds.\n");

    // Application logic
    int counter = 0;
    while (1) {
        // Simulate some application logic
        printf("Counter: %d\n", counter);
        counter++;
        sleep(1); // Wait for 1 second
    }

    return 0;
}
