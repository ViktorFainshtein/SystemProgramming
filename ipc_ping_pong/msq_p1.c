#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    mqd_t mqd = 0;

    mqd = mq_open("/msq", O_CREAT | O_WRONLY,  0600, NULL);

    if (mqd == -1)
    {
        perror("mq_open");
        exit(1);
    }

    mq_send(mqd, "HELLO THERE,", 13, 10);
    mq_send(mqd, "GENERAL", 8, 10);
    mq_send(mqd, "CANNOLI", 8, 10);
    
    mq_close(mqd);

    return 0;
}