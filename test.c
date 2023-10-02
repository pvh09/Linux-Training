// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>
// #include <unistd.h>

// // Define the maximum number of lines in the file
// #define MAX_LINES 50

// // Global variables
// int current_line = 0;  // Track the current line being read
// int lines[MAX_LINES];  // Store the lines read from the file
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// // Function to read a line from the file
// int readLine(FILE *file) {
//     int line;
//     if (fscanf(file, "%d", &line) != 1) {
//         return -1;  // End of file or error
//     }
//     return line;
// }

// // Thread function
// void *readFile(void *arg) {
//     FILE *file = (FILE *)arg;

//     while (1) {
//         pthread_mutex_lock(&mutex);  // Lock the mutex

//         if (current_line >= MAX_LINES) {
//             pthread_mutex_unlock(&mutex);  // Release the mutex
//             break;  // Exit if we've read all lines
//         }

//         int line = readLine(file);
//         if (line != -1) {
//             lines[current_line++] = line;
//             printf("Thread %ld read line %d\n", pthread_self(), line);
//         }
//         sleep(1);
//         pthread_mutex_unlock(&mutex);  // Release the mutex
//     }

//     return NULL;
// }

// int main() {
//     FILE *file = fopen("input.txt", "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         return 1;
//     }

//     pthread_t threads[3];
//     int i;

//     for (i = 0; i < 3; i++) {
//         pthread_create(&threads[i], NULL, readFile, file);
//     }

//     for (i = 0; i < 3; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     fclose(file);

//     // Print the lines read from the file
//     printf("Lines read from the file:\n");
//     for (i = 0; i < MAX_LINES; i++) {
//         if (lines[i] != 0) {
//             printf("%d\n", lines[i]);
//         }
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>

// #define MAX_SIZE 50

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// int data[MAX_SIZE];
// int count = 0;

// void *read_file(void *arg) {
//     FILE *fp;
//     char *filename = (char *)arg;
//     char buffer[100];

//     fp = fopen(filename, "r");
//     if (fp == NULL) {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }

//     while (fgets(buffer, sizeof(buffer), fp) != NULL) {
//         pthread_mutex_lock(&mutex);
//         if (count < MAX_SIZE) {
//             printf("Thread %ld: %s", pthread_self(), buffer);
//             data[count++] = atoi(buffer);
//         }
//         pthread_mutex_unlock(&mutex);
//     }

//     fclose(fp);
//     pthread_exit(NULL);
// }

// int main() {
//     pthread_t threads[3];
//     int rc;
//     char *filename = "input.txt";

//     rc = pthread_create(&threads[0], NULL, read_file, (void *)filename);
//     if (rc) {
//         perror("Error creating thread");
//         exit(EXIT_FAILURE);
//     }

//     rc = pthread_create(&threads[1], NULL, read_file, (void *)filename);
//     if (rc) {
//         perror("Error creating thread");
//         exit(EXIT_FAILURE);
//     }

//     rc = pthread_create(&threads[2], NULL, read_file, (void *)filename);
//     if (rc) {
//         perror("Error creating thread");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < 3; i++) {
//         rc = pthread_join(threads[i], NULL);
//         if (rc) {
//             perror("Error joining thread");
//             exit(EXIT_FAILURE);
//         }
//     }


//     pthread_mutex_destroy(&mutex);
//     pthread_exit(NULL);
// }


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_SIZE 50

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int data[MAX_SIZE];
int count = 0;

int getRandomThreadID() {
    return rand() % 3 + 1; // Trả về 1, 2 hoặc 3 ngẫu nhiên
}

void *read_file(void *arg) {
    FILE *fp;
    char *filename = "input.txt";
    char buffer[100];

    int thread_id = *((int *)arg); // Lấy giá trị ID của thread từ tham số truyền vào

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int current_thread_id;

        pthread_mutex_lock(&mutex);
        if (count < MAX_SIZE) {
            current_thread_id = getRandomThreadID();

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

int main() {
    srand(time(NULL)); // Khởi tạo seed cho hàm rand()

    pthread_t threads[3];
    int rc;
    
    int thread_ids[] = {1, 2, 3};

    rc = pthread_create(&threads[0], NULL, read_file, &thread_ids[0]);
    if (rc) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    rc = pthread_create(&threads[1], NULL, read_file, &thread_ids[1]);
    if (rc) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    rc = pthread_create(&threads[2], NULL, read_file, &thread_ids[2]);
    if (rc) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
