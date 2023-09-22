#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler1(int num){
    printf("\n Im signal handler 1: %d\n", num);
    //while(1);
    //exit(EXIT_SUCCESS);
}

void sig_handler2(int num){
    printf("\n Im signal handler 2: %d\n", num);
}

void sig_handler3(int num){
    printf("\n Im signal handler 3: %d\n", num);
    exit(EXIT_SUCCESS);
}

int main()
{
    if(signal(SIGINT, sig_handler1) == SIG_ERR){
        fprintf(stderr, "Can not handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    // signal(SIGKILL, sig_handler2);
    // signal(SIGTERM, sig_handler3);
    // sleep(4);
    // kill(getpid(), SIGINT);

    printf("\n The procress_id: %d\n", getpid());
    while (1)
    {
        printf("Hello\n");
        sleep(2);
    }
    
}