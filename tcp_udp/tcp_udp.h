#ifndef __TCP_UDP_H__
#define __TCP_UDP_H__

#include <arpa/inet.h>  /*sockaddr_in*/

int CreateTcpServerSocket(int port);

int CreateUdpServerSocket(int port);

int ConnectToServer(int *sockfd, struct sockaddr_in *servaddr);

void TcpServerFunc(int connfd);

int TcpSelectServerFunc(int connfd);

int max(int a, int b, int c);



#endif