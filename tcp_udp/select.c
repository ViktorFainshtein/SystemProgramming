
#include <stdio.h>
#include <sys/select.h> /* select */
#include <unistd.h>     /* read */
#include <string.h>     /* strlen */


int main()
{
    fd_set readfd;
    int sock = 0;
    char buffer1[1024];
    char buffer2[1024];

    FD_ZERO(&readfd);
    FD_SET(sock, &readfd);


    while (1)
    {
        select(0+1, &readfd, NULL, NULL, NULL);
        read(sock, &buffer1, 1024);
        printf("%s\n", buffer1);
    }
    


    return 0;
}