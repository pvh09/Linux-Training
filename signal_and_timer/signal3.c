#define _POSIX_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void sig_handler1(int signum)
{
    printf("\n Im signal handler 1: %d\n", signum);
    exit(EXIT_SUCCESS);
}

int main()
{
    // time_t start, finish
    sigset_t new_set, old_set;
    if (signal(SIGINT, sig_handler1) == SIG_ERR)
    {
        fprintf(stderr, "Can not handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&new_set);
    sigemptyset(&old_set);

    //sigaddset(&new_set, SIGINT);
    // sigaddset(&new_set, SIGCHLD);

     printf("Checking signal set:\n");

    // for (int i = 1; i < SIGRTMAX + 1; i++) {
    //     if (sigismember(&new_set, i)) {
    //         printf("Signal %d is in the set.\n", i);
    //     }
    // }

    if (sigprocmask(SIG_SETMASK, &new_set, &old_set) == 0)
    {
        // sigprocmask(SIG_SETMASK, NULL, &old_set)
        if (sigismember(&new_set, SIGINT) == 1)
        {
            printf("SIGINT is exist!\n");
        }
        else if (sigismember(&new_set, SIGINT) == 0)
        {
            printf("SIGINT is not exist!!\n");
        }
    }
    while (1);

    return 0;
    
}