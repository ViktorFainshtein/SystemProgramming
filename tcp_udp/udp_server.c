/* Server side implementation of UDP client-server model */
#include "tcp_udp.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
   
#define PORT 5000
#define MAXLINE 1024
   
/* Driver code */
int main(int argc, char *argv[]) 
{
    int sockfd = 0;
    char buffer[MAXLINE];
    const char *hello = "Pong from server";
    int n = 0;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    
    int port = 0;
    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }

    sockfd = CreateUdpServerSocket(port);
       
    while(1)
    {
        len = sizeof(cliaddr);  /*len is value/result*/
    
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
        buffer[n] = '\0';
        
        printf("Server : %s\n", buffer);
        
        sendto(sockfd, (const char *)hello, strlen(hello)+1, 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);

        printf("Server: Pong message sent.\n");        
    }

       
    return 0;
}