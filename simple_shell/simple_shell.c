
#include <stdio.h>  /*printf fgets*/
#include <stdlib.h> /*malloc free exit*/
#include <sys/wait.h>   /*wait*/
#include <unistd.h> /*fork exec */
#include <string.h> /*strcmp strtok*/

#define BUFFER_SIZE 1024


int main(int argc, char **argv)
{
    int buffer_size = BUFFER_SIZE;
    char *buffer = NULL;
    char *command = NULL;
    char *add_on = NULL;
    int i = 0;
    pid_t pid;
    
    buffer = (char *)malloc(buffer_size);
    if(!buffer)
    {
        exit(1);
    }

    command = malloc(buffer_size);
    add_on = malloc(buffer_size);

    while(1)
    {
        /*take input*/
        /*if(fgets(buffer, buffer_size, stdin) == NULL)
        {
            printf("fgets faild\n");
            return 1;
        }*/
        scanf("%s", buffer);
        /*parse here*/
        /*while(*buffer != ' ')
        {
            *(command + i) = *buffer;
            ++buffer;
            ++i;
        }*/


        /*exit in case of exit*/
        if(0 == strcmp(buffer, "exit"))
        {
            exit(0);
        }
        /*other wise fork*/
        pid = fork();
        /*if failed return*/
        if(-1 == pid)
        {
            printf("failed to fork\n");
            return 1;
        }
        /*otherwise execute*/
        else if(0 == pid)
        {
            if(execlp(buffer, buffer, NULL))
            {
                printf("execlp failed\n");
            }
        }
        /*wait for child process to finish*/
        else
        {
            wait(NULL);
        }

    }

    return 0;
}

