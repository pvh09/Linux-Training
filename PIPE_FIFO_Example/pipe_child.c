/*
NOTE: DUNG PIPE NHIEU TRONG RELATE-PROCESS (parent - child)
    Tao ra tien trinh con qua fork()
    - Tao pipe truoc khi goi fork() thi co the share voi nhau

    TH1: Neu co data in pipe: in ra data
        + in ra het data thi program bi block
    TH2: Neu dau write() bi dong
        + neu chi dong dau wrie cua cha hoac con thi program
        van bi block vi 1van con dau write
        + neu do ca 2 dau write => print read end of file
    Q: kiem tra khi ko viet vao fil
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MSG_SIZE 16

char *msg1 = "hello, world #1";
char *msg2 = "hello, world #1";
char *msg3 = "hello, world #1";

static void func(int signum)
{
    wait(NULL);
    printf("Child process termainated..");
}

int main(int argc, char const *argv[])
{
    char in_buff[MSG_SIZE];
    int num_thread = 0;
    int fds[2];
    pid_t chid_pid;

    if (pipe(fds) == -1)
    {
        printf("Pipe unccessfully\n");
        exit(1);
    }

    chid_pid = fork();
    if (chid_pid >= 0)
    {
        if (chid_pid == 0)
        {
            // do something
            printf("I am a child\n"); // reader
            // if (close(fds[1]) == -1) // write end unused
            // {
            //     printf("Cose fd[1] faild\n");
            // }

            while (1)
            {
                num_thread = read(fds[0], in_buff, MSG_SIZE);
                // printf("Unblock read()\n");
                if (num_thread == -1)
                {
                    printf("Read() failed!!\n");
                    exit(0);
                }
                else if (num_thread == 0)
                {
                    printf("Pipe end of file\n");
                    break;
                }
                else
                {
                    printf("Msg: %s\n", in_buff);
                }
            };
        }
        else
        {
            signal(SIGCHLD, func); // prevent zombie proccess
            printf("I am a Parent\n");

            //writer
            if (close(fds[1]) == -1) // read end is unused
                printf("Close fds[0] failed!!\n");

            write(fds[1], msg1, MSG_SIZE);
            write(fds[1], msg2, MSG_SIZE);
            write(fds[1], msg3, MSG_SIZE);

            if (close(fds[1]) == -1)
                printf("Close fds[0] failed!!\n");

            while (1)
                ;
        }
    }
    else
    {
        printf("Fork() unsuccessfully!!!\n");
    }
}