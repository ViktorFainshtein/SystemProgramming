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
    const char* message_0 = "Hello World";
    int shm_fd = 0;
    void* ptr = NULL;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);            /* create the shared memory object */
    ftruncate(shm_fd, SIZE);                                    /* configure the size of the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);     /* memory map the shared memory object */
    
    sprintf((char *)ptr, "%s", message_0);                      /* write to the shared memory object */

    munmap(ptr, SIZE);
/*     shm_unlink(name); */

    return 0;
}