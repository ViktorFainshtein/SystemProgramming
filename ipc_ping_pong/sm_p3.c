#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>



int main()
{
    const int SIZE = 4096;
    const char* name = "sm";
    int shm_fd = 0;
    void* ptr = NULL;

    shm_fd = shm_open(name, O_RDONLY, 0666);
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    printf("P3: %s\n", (char *)ptr);

    return 0;
}