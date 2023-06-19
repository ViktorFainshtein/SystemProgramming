#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pfds1[2];
    int pfds2[2];
    char buff1[30];
    int pid = 0;

    if(pipe(pfds1) == -1)
    {
        printf("failed to create a pipe\n");
        exit(0);
    }

    if(pipe(pfds2) == -1)
    {
        printf("failed to create a pipe\n");
        exit(0);
    }

    pid = fork();

    while(1)
    {
        if(pid != 0)     /*child proc*/
        {

            write(pfds1[1], "ping", 5);
            read(pfds2[0], buff1, 5);
            printf("%s\n", buff1);
        }
        else            /*parent proc*/
        {
            read(pfds1[0], buff1, 5);
            write(pfds2[1], "pong", 5);
            printf("%s\n", buff1);
        }
    }


    return 0;
}