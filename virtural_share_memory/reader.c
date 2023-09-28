#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

void main()
{
    int shm_fd;
    //char read_buff[1024];
    void* ptr = NULL;

    shm_fd = shm_open("Hello", O_RDONLY, 0666);
    ptr = mmap(0, 4096, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("%s", (char*)ptr);

    shm_unlink("Hello");
}