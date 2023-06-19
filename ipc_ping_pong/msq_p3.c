#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    mqd_t mqd = 0;
    int attr_ret = 0;
    struct mq_attr attr;
    char *buffer = NULL;
    unsigned int priority = 0;

    mqd = mq_open ("/msq", O_RDONLY);
    assert(mqd != -1);

    attr_ret = mq_getattr(mqd, &attr);
    assert(attr_ret != -1);

    buffer = calloc(attr.mq_msgsize, 1);
    assert (buffer != NULL);

    if ((mq_receive(mqd, buffer, attr.mq_msgsize, &priority)) == -1)
    {
        printf ("Failed to receive message\n");
    }
    else
    {
        printf ("Received [priority %u] In P3: '%s'\n", priority, buffer);
    }

    free(buffer);
    buffer = NULL;
    mq_close(mqd);

    return 0;
}