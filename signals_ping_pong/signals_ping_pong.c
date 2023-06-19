#define _XOPEN_SOURCE 700
#include <stdio.h>     /*printf fgets*/
#include <stdlib.h>    /*malloc free exit*/
#include <sys/wait.h>  /*wait*/
#include <unistd.h>    /*fork exec */
#include <string.h>    /*strcmp strtok*/
#include <signal.h>    /*signal*/
#include <sys/types.h> /*kill*/

volatile int lock = 0;

#if 0   /*EX - 1*/
void SignalHandler(int usr_sig);


/*fork - seind SIGUSR1 from papa to child and return SIGUSR2 from child to papa*/
int main()
{
    pid_t pid;
    pid_t parent_pid;

    struct sigaction handle;
    handle.sa_handler = SignalHandler;


    parent_pid = getpid();
    printf("parent pid = %d\n", parent_pid);

    sigaction(SIGUSR1, &handle,NULL);
    sigaction(SIGUSR2, &handle,NULL);
    pid = fork();

    /*other wise fork*/
    /*if failed return*/
    if (-1 == pid)
    {
        printf("failed to fork\n");
        return 1;
    }
    /*otherwise execute*/
    else if (0 == pid)
    {
        while (1)
        {
            while (lock == 0);
            lock = 0;
            printf("Pong!\n");
            kill(getppid(), SIGUSR2);
        }
    }
    /*wait for child process to finish*/
    else
    {
        while (1)
        {
            while (lock == 1);
            lock = 1;
            printf("Ping!\n");
            kill(pid, SIGUSR1);
        }
    }

    return 0;
}

void SignalHandler(int usr_sig)
{

    if(SIGUSR1 == usr_sig)
    {
        lock = 1;
    }
    if(SIGUSR2 == usr_sig)
    {
        lock = 0;
    }
}
#endif
#if 0   /*EX - 1 v2*/
void SignalHandler(int usr_sig);


/*fork - seind SIGUSR1 from papa to child and return SIGUSR2 from child to papa*/
int main()
{
    pid_t pid;
    pid_t parent_pid;

    struct sigaction handle;
    handle.sa_handler = SignalHandler;


    parent_pid = getpid();
    printf("parent pid = %d\n", parent_pid);

    sigaction(SIGUSR1, &handle,NULL);
    sigaction(SIGUSR2, &handle,NULL);
    pid = fork();

    /*other wise fork*/
    /*if failed return*/
    if (-1 == pid)
    {
        printf("failed to fork\n");
        return 1;
    }
    /*otherwise execute*/
    else if (0 == pid)
    {
        while (1)
        {
            while (lock == 1);
            lock = 1;
            printf("Pong!\n");
            kill(getppid(), SIGUSR2);
        }
    }
    /*wait for child process to finish*/
    else
    {
        while (1)
        {
            kill(pid, SIGUSR1);
            while (lock == 1);
            lock = 1;
            printf("Ping!\n");
        }
    }

    return 0;
}

void SignalHandler(int usr_sig)
{
    lock = 0;
}
#endif

#if 1   /*EX - 2*/
void SignalHandler(int usr_sig);

int main()
{
    pid_t pid;
    pid_t parent_pid;

    struct sigaction handle;
    handle.sa_handler = SignalHandler;

    parent_pid = getpid();
    printf("parent pid = %d\n", parent_pid);

    sigaction(SIGUSR2, &handle,NULL);
    pid = fork();

    /*other wise fork*/
    /*if failed return*/
    if (-1 == pid)
    {
        printf("failed to fork\n");
        return 1;
    }
    /*otherwise execute*/
    else if (0 == pid)
    {
        /*EXEC!!!*/
        if(execlp("./a.out", "a.out", NULL))
        {
            printf("execlp failed\n");
        }

    }
    /*wait for child process to finish*/
    else
    {

        while (1)
        {
            while (lock == 0);
            lock = 0;
            printf("Ping!\n");
            kill(pid, SIGUSR1);
        }
    }
    return 0;
}

void SignalHandler(int usr_sig)
{
    lock = 1;
}
#endif

#if 0   /*EX - 4*/

pid_t pid;

void SignalHandler(int usr_sig, siginfo_t *info, void *u_con);

int main()
{
	int i = 0;
    pid_t parent_pid;

    struct sigaction handle;    
    handle.sa_sigaction = &SignalHandler;
    handle.sa_flags = SA_SIGINFO;

    parent_pid = getpid();
    printf("parent pid = %d\n", parent_pid);

    sigaction(SIGUSR2, &handle ,NULL);

    /*other wise fork*/
    /*if failed return*/

    /*otherwise execute*
    /*wait for child process to finish*/

    while(1)
    {
        while (lock == 0);
        lock = 0;
        printf("Ping!\n");
        kill(pid, SIGUSR1);
    }

    return 0;
}

void SignalHandler(int usr_sig, siginfo_t *info, void *u_con)
{
    (void)u_con;
    (void)usr_sig;
    pid = info->si_pid;
    lock = 1;
}
#endif
