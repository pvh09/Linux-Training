#include "mutex_sync.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int data[MAX_SIZE];
int count = 0;

static int get_random_thread_id() {
    return rand() % 3 + 1;
}

static void *read_file(void *arg) {
    FILE *fp;
    char *filename = "input.txt";
    char buffer[MAX_FILE];

    int thread_id = *((int *)arg); // get thread ID 

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int current_thread_id;

        pthread_mutex_lock(&mutex);
        if (count < MAX_SIZE) {
            current_thread_id = get_random_thread_id();

            if (current_thread_id == thread_id) {
                printf("Thread %d: %s", thread_id, buffer);
                data[count++] = atoi(buffer);
            }
        }
        pthread_mutex_unlock(&mutex);
        
    }

    fclose(fp);
    pthread_exit(NULL);
}

void init_mutex()
{
    pthread_t threads[MAX_THREADS];
    int rc = 0;
    int thread_ids[] = {1, 2, 3};

    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        rc = pthread_create(&threads[0], NULL, read_file, &thread_ids[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}