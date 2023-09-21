#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utmpx.h>
#include <time.h> // Include the time.h header

// Function to initialize the daemon process
void init_daemon() {
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();

    // An error occurred
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then we can exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory to a safe location
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // Close the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    // Daemonize the process
    init_daemon();

    // Set the log file path (you can change this to your preferred location)
    const char *log_file_path = "/home/pvh/Working_space/Linux-Training/3_Processes_and_Threads/daemon_process/login.log";

    // Open the log file for writing (create if it doesn't exist)
    int log_fd = open(log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        perror("Failed to open log file");
        exit(1);
    }

    // Redirect stdout and stderr to the log file
    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);

    // Main daemon logic
    while (1) {
        // Open the utmpx database for reading
        setutxent();

        // Read and write login information to the log file
        struct utmpx *entry;
        while ((entry = getutxent()) != NULL) {
            if (entry->ut_type == USER_PROCESS) {
                // Get the current time and date
                time_t raw_time;
                struct tm *time_info;
                time(&raw_time);
                time_info = localtime(&raw_time);

                // Format the timestamp with date and time
                char timestamp[64];
                strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

                printf("[%s] User %s logged in from %s\n", timestamp, entry->ut_user, entry->ut_host);
                fflush(stdout); // Flush stdout to ensure data is written immediately
            }
        }
        endutxent();

        sleep(60); // Sleep for 60 seconds before the next iteration
    }
    close(log_fd);

    return 0;
}
