#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(){
    printf("\n>> Do not allow enter CTR + C to end program!\n");
    while(1);
}

int main()
{
    printf("The procress_id: %d\n", getpid());
    if(signal(SIGINT, sig_handler) == SIG_ERR){
        fprintf(stderr, "Can not handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    
    while(1);
    return 0;
}
