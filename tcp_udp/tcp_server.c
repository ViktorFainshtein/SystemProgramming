#include "tcp_udp.h"

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h> /* bzero() */
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> /* read(), write(), close() */

#define MAX 80
#define PORT 5000
#define SA struct sockaddr
   
/* Driver function */
int main(int argc, char *argv[]) 
{
    int sockfd = 0, connfd = 0, len = 0;
    struct sockaddr_in servaddr, cli;

    int port = 0;
    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }

    sockfd = CreateTcpServerSocket(port);
   
    /* Now server is ready to listen and verification */
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening..\n");
    }

    len = sizeof(cli);
   
    /* Accept the data packet from client and verification */
    connfd = accept(sockfd, (SA*)&cli, (socklen_t *)&len);
    if (connfd < 0) 
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
    {
        printf("server accept the client...\n");
    }
   
    /* Function for chatting between client and server */
    TcpServerFunc(connfd);
   
    /* After chatting close the socket */
    close(sockfd);

    return 0;
}