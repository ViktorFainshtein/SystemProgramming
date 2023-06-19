#include "tcp_udp.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <linux/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>     /* read */

#include <netdb.h>
#include <netinet/in.h>
#include <strings.h> /* bzero() */

#define IP_FOUND_ACK "IP_FOUND_ACK"
#define PORT 5000

int main(int argc, char *argv[]) 
{
    int udp_socket = 0, tcp_socket = 0, connfd = 0;
    int count = 0;
    int ret = 0;
    char buffer[1024];
    char *msg1 = "Pong";
    int yes = 1;
    struct sockaddr_in server_addr, client_tcp_addr, client_udp_addr;
    socklen_t len_udp, len_tcp;
    fd_set readfd;
    fd_set temp;

    int udp_port = 0;
    int tcp_port = 0;

    udp_port = PORT;
    tcp_port = PORT+1;

    if(argc > 2)
    {
        udp_port = atoi(argv[1]);
        tcp_port = atoi(argv[2]);
    }

    udp_socket = CreateUdpServerSocket(udp_port);
    tcp_socket = CreateTcpServerSocket(tcp_port);

/*     ret = setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
    if(ret == -1) 
    {
        perror("setsockopt error");
        return 0;
    } */

    if ((listen(tcp_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening..\n");
    }

    FD_ZERO(&readfd);
    FD_SET(0, &readfd);    
    FD_SET(udp_socket, &readfd);
    FD_SET(tcp_socket, &readfd);

    while(1)
    {
        temp = readfd;
        ret = select((max(udp_socket, tcp_socket, connfd) + 1), &temp, NULL, NULL, NULL);      /*set max +1 (num of sockets)*/

        if(ret > 0) 
        {

            if(FD_ISSET(0, &temp))
            {
                read(0, &buffer, 1024);
                printf("%s\n", buffer);
            }
            else if(FD_ISSET(udp_socket, &temp))
            {
                len_udp = sizeof(client_udp_addr);
                count = recvfrom(udp_socket, (char *)buffer, 1024, MSG_WAITALL, 
                                (struct sockaddr*)&client_udp_addr,(socklen_t *)&len_udp);
                
                buffer[count] = '\0';
                
                printf("Server : %s\n", buffer);
                sendto(udp_socket, (const char *)msg1, strlen(msg1)+1, MSG_CONFIRM, 
                                (const struct sockaddr *) &client_udp_addr,len_udp);
            }
            else if(FD_ISSET(tcp_socket, &temp))
            {
                len_tcp = sizeof(client_tcp_addr);
                connfd = accept(tcp_socket, (struct sockaddr*)&client_tcp_addr, (socklen_t *)&len_tcp);
                FD_SET(connfd, &readfd);
                
                if (connfd < 0) 
                {
                    printf("server accept failed...\n");
                    exit(0);
                }
                else
                {
                    printf("server accept the client...\n");
                }
            }
            else if(FD_ISSET(connfd, &temp))
            {
                int res = 0;
                res = TcpSelectServerFunc(connfd);
                
                if(res == 1)
                {
                    close(connfd);
                    FD_CLR(connfd, &readfd);
                }
            }
        }
    }

    return 0;
}