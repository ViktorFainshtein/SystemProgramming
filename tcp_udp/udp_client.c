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
    const char *hello = "Ping From Client";
    int n = 0;
    struct sockaddr_in servaddr;
    socklen_t len;
   
    int port = 0;
    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }

    /* Creating socket file descriptor */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    /* Filling server information */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
       
    sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    printf("Client: Ping message sent\n");
        
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    
    buffer[n] = '\0';
    
    printf("Clinet Recived: %s\n", buffer);

   
    close(sockfd);
    return 0;
}