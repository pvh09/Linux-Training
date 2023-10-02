#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10
int main(){
    char *buffer = (char*)mmap(NULL, sizeof(int)*BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_t *mutex = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *full_buffer = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *empty_buffer = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(mutex, 1, 1); // khoa 
    sem_init(full_buffer, 1, 0); // neu full = 10, bang day, defaut value = 0
    sem_init(empty_buffer, 1, BUFFER_SIZE); // neu empty = 10, bang trong, defaut value = 10

    // forking

    pid_t producer = fork();
    pid_t consumer = fork();

    if(producer == 0){
        while (1)
        {
            sem_wait(empty_buffer);
            sem_wait(mutex);

            printf("Producer created successfull\n");

            sem_post(full_buffer);
            sem_post(mutex);

            srand(time(NULL));
            sleep(rand() % 5);
        }
    }

    if(consumer == 0){
        while (1)
        {
            sem_wait(full_buffer);
            sem_wait(mutex);

            printf("Consumer completed successfull\n");

            sem_post(mutex);
            sem_post(empty_buffer);
            

            srand(time(NULL));
            sleep(3 + rand() % 5);
        }
    }
    //parent
    else {
        while (1)
        {
            sleep(10);
            int taken_slots;
            sem_getvalue(full_buffer, &taken_slots);

            printf("Intems in buffer: %d/%d\n", taken_slots, BUFFER_SIZE);
        }
    }
    
    
}