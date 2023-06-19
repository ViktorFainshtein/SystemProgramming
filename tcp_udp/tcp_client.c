#include "tcp_udp.h"

#include <arpa/inet.h> /* inet_addr() */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* bzero() */
#include <sys/socket.h>
#include <unistd.h> /* read(), write(), close() */

#define MAX 80
#define PORT 5000
#define SA struct sockaddr

void ClientFunc(int sockfd)
{
    char buff[MAX];
    char *ping = "Ping";

    /* for (;;) */                                    /* while true - endless loop */
    /* { */
        bzero(buff, sizeof(buff));              /* set buffer to 0's */
        write(sockfd, ping, strlen(ping)+1);    /* write to server "ping" */

        bzero(buff, sizeof(buff));              /* set buffer to 0's */
        read(sockfd, buff, sizeof(buff));       /* read into buffer */
        
        printf("From Server : %s\n", buff);
    /* } */
}
 
int main(int argc, char *argv[]) 
{
    int sockfd = 0, connfd = 0;
    struct sockaddr_in servaddr, cli;
    int res = 0;

    int port = 0;
    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }
 
    /* socket create and verification */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) 
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully created..\n");
    }

    bzero(&servaddr, sizeof(servaddr)); /* init servaddr struct to 0's*/
 
    /* assign IP, PORT */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);      /* assigen home address*/
    servaddr.sin_port = htons(port);
    
    res = ConnectToServer(&sockfd, &servaddr);
 
    /* function for chat */
    ClientFunc(sockfd);
 
    /* close the socket */
    close(sockfd);

    return 0;
}