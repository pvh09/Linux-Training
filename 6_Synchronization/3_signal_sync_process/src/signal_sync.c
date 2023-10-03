#include "signal_sync.h"

int p_counter = 0;

static void write_to_file(char *file, int counter)
{
    // Open file
    FILE *fp = NULL;
    fp = fopen(file, "a");
    if (fp == NULL)
    {
        printf("Error openning file to write\n");
        exit(1);
    }

    // Append to file
    fprintf(fp, "Process %i: %i\n", getpid(), counter);
    fclose(fp);
}

static int get_number_of_last_line(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    int last_number = 0;
    int temp_number = 0;
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_SIZE];     // Adjust the buffer size as needed
    char last_line[MAX_SIZE]; // To store the last line

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Copy the current line to lastLine
        strcpy(last_line, line);
        if (sscanf(last_line, "Process %*d: %d", &temp_number) == 1)
        {
            last_number = temp_number;
        }
        else
        {
            printf("Number not found in the last line.\n");
        }
    }
    fclose(file);
    return last_number;
}

void parent_handler(int signo)
{
    if (signo == SIGUSR2)
    {
        // Parent process (Process A) executes this code
        printf("Parent Process (A) received signal SIGUSR2\n");
    }
}

void child_handler(int signo){}

void child_process()
{
    
    while (1)
    {
        // Wait for a signal from Process A to continue
        pause();
        sleep(1);
        p_counter = get_number_of_last_line(OUTPUT_FILE);
        p_counter++;
        write_to_file(OUTPUT_FILE, p_counter);
        printf("Process B - Counter: %d\n", p_counter);

        // Send a signal to Process A to increment the counter
        kill(getppid(), SIGUSR1);
    }
}

void parent_process()
{
    FILE *fp;
    fp = fopen(OUTPUT_FILE, "a");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Register the signal handler for SIGUSR1 in Process B
    if (signal(SIGUSR1, child_handler) == SIG_ERR)
    {
        perror("Error registering child signal handler");
        exit(EXIT_FAILURE);
    }

    // Register the signal handler for SIGUSR2 in Process A
    if (signal(SIGUSR2, parent_handler) == SIG_ERR)
    {
        perror("Error registering parent signal handler");
        exit(EXIT_FAILURE);
    }

    // Fork a child process (Process B)
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        // Child process (Process B)
        // Set up signal handlers before entering the loop
        child_process();
    }
    else
    {
        while (1)
        {
            sleep(1);
            p_counter = get_number_of_last_line(OUTPUT_FILE);
            p_counter++;
            write_to_file(OUTPUT_FILE, p_counter);
            printf("Process A - Counter: %d\n", p_counter);

            // Send a signal to Process B to increment the counter
            kill(child_pid, SIGUSR1);

            // Wait for a signal from Process B to continue
            pause();
        }
    }
}
