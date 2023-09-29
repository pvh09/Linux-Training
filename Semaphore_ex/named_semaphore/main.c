#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
 #include <fcntl.h>

#define POSIX_SEM_NAMED "/named_app"
#define R_W_PERM    0666
#define SEM_WAITTING_TIMEOUT    10000
#define MSECS_IN_SEC    1000

int main(int argc, char* argv[]){
    char c;
    int ret = -1;
    int current_value;
    sem_t *sem;
    struct timespec timeout;

    sem = sem_open(POSIX_SEM_NAMED, O_CREAT | O_EXCL, R_W_PERM, 1);
    
    if(sem == SEM_FAILED){
        if(errno != EEXIST) {
            printf("Failed to open semaphore error: %s\n", strerror(errno));
            return -1;
        }

        printf("%s, Reading avaiable semaphore.\n", argv[0]);
        sem = sem_open(POSIX_SEM_NAMED, 0);
        if(errno != EEXIST) {
            printf("Failed to open semaphore error: %s\n", strerror(errno));
            return -1;
        }
    }

    // get current semaphore
    sem_getvalue(sem, &current_value);
    printf("current semaphore value = %d.\n", current_value);

    // locking with time out
    if(clock_gettime(CLOCK_REALTIME, &timeout)){
        printf("Failed to get current time error: %s\n", strerror(errno));
        return -1;
    }

    timeout.tv_sec += SEM_WAITTING_TIMEOUT / MSECS_IN_SEC;
    ret = sem_timedwait(sem, &timeout);
    if(ret == -1){
        printf("Failed to get current time error: %s\n", strerror(errno));
        return -1;
    }

    printf("\n%s, Please type any character to exit ...\n", argv[0]);
    getchar();

    ret = sem_close(sem);
    if(ret == -1){
        printf("%s, Failed to close semaphore error: %s\n", argv[0], strerror(errno));
        return -1;
    }

    sem_unlink(&sem);

    return ret;
}