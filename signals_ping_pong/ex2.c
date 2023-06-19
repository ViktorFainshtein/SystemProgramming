#define _XOPEN_SOURCE 700
#include <stdio.h>     /*printf fgets*/
#include <stdlib.h>    /*malloc free exit*/
#include <sys/wait.h>  /*wait*/
#include <unistd.h>    /*fork exec */
#include <string.h>    /*strcmp strtok*/
#include <signal.h>    /*signal*/
#include <sys/types.h> /*kill*/

int lock = 1;

void SignalHandler(int usr_sig);

int main()
{
    struct sigaction handle;
    handle.sa_handler = SignalHandler;

    sigaction(SIGUSR1, &handle,NULL);
    
    while (1)
    {
        kill(getppid(), SIGUSR2);
        while (lock == 1);
        lock = 1;
        printf("Pong!\n");
    }

    return 0;
}

void SignalHandler(int usr_sig)
{
    lock = 0;
}