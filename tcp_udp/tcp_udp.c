/*------------------------------ Header --------------------------------------*/
#include "tcp_udp.h"
/*------------------------------ tcp includes --------------------------------*/
#include <arpa/inet.h> /* inet_addr() */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* bzero() */
#include <sys/socket.h>
#include <unistd.h> /* read(), write(), close() */
/*------------------------------ udp includes --------------------------------*/
#include <sys/types.h>
#include <netinet/in.h>
/*------------------------------ defines -------------------------------------*/
#define SA struct sockaddr
#define MAX 80
#define MAXLINE 1024
/*----------------------------------------------------------------------------*/

int CreateTcpServerSocket(int port)
{
    int sockfd = 0;
    struct sockaddr_in servaddr;
   
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

    bzero(&servaddr, sizeof(servaddr));
   
    /* assign IP, PORT */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
   
    /* Binding newly created socket to given IP and verification */
    if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully binded..\n");
    }

    return sockfd;
}

int CreateUdpServerSocket(int port)
{
    int sockfd = 0;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    /* Creating socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    /* Filling server information */
    servaddr.sin_family    = AF_INET; /* IPv4 */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
       
    /* Bind the socket with the server address */
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

int ConnectToServer(int *sockfd, struct sockaddr_in *servaddr)
{
    int connectfd = 0;
    connectfd = connect(*sockfd, (SA*)servaddr, sizeof(*servaddr));
    if (connectfd != 0) 
    {
        printf("connection with the server failed...\n");
        return -1;
    }
    else
    {
        printf("connected to the server..\n");
        return connectfd;
    }
}
/* Function designed for chat between client and server. */
void TcpServerFunc(int connfd)
{
    char buff[MAX];
    char *pong = "Pong";

    bzero(buff, MAX);                   /* set buffer to 0's*/
    read(connfd, buff, sizeof(buff));

    printf("From client: %s\n", buff);

    bzero(buff, MAX);                       /* set buffer to 0's*/
    write(connfd, pong, strlen(pong)+1);
}

int TcpSelectServerFunc(int connfd)
{
    char buff[MAX];
    char *pong = "Pong";
    int res = 0;

    bzero(buff, MAX);                   /* set buffer to 0's*/
    res = read(connfd, buff, sizeof(buff));

    printf("From client: %s\n", buff);

    bzero(buff, MAX);                       /* set buffer to 0's*/
    write(connfd, pong, strlen(pong)+1);

    return (0 == res); 
}

int max(int a, int b, int c)
{
    if(a>b && a>c)
    {
        return a;
    }
    else if(b>a && b>c)
    {
        return b;
    }
    return c;
}