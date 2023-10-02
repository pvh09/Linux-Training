#include "semaphore_sync.h"

int g_common_var = 0;
sem_t g_sem1, g_sem2;

void *thread_tunction(void *arg);

void init_semaphore()
{
    // Initialize semaphores
    sem_init(&g_sem1, 0, 1);
    sem_init(&g_sem2, 0, 1);

    // Create three threads
    pthread_t thread[MAX_THREADS];
    int thread_ids[] = {1, 2, 3};

    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        pthread_create(&thread[i], NULL, thread_tunction, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&g_sem1);
    sem_destroy(&g_sem2);
}

// Function executed by each thread
void *thread_tunction(void *arg)
{
    int thread_id = *((int *)arg);

    char file_name[MAX_FILE_NAME];
    FILE *file;

    for (int i = 0; i < MAX_VALUE; i++)
    {
        int random_file = rand() % 2 + 1;
        sprintf(file_name, "output%d.txt", random_file);
        file = fopen(file_name, "a");

        if (file == NULL)
        {
            perror("Error opening file");
            return NULL;
        }

        if (random_file == 1)
        {
            sem_wait(&g_sem1);
        }
        else
        {
            sem_wait(&g_sem2);
        }

        g_common_var++;

        fprintf(file, "Thread %d: %d\n", thread_id, g_common_var);
        printf("Thread %d: %d is wwritten into %s\n", thread_id, g_common_var, file_name);
        sleep(1);

        if (random_file == 1)
        {
            sem_post(&g_sem1);
        }
        else
        {
            sem_post(&g_sem2);
        }

        fclose(file);
    }

    return NULL;
}