#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <linux/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>

#define IP_FOUND_ACK "IP_FOUND_ACK"
#define PORT 5000

int main(int argc, char *argv[]) 
{
    int sock = 0;
    int yes = 1;
    struct sockaddr_in broadcast_addr;
    struct sockaddr_in server_addr;
    int addr_len = 0;
    int count = 0;
    int ret = 0;
    fd_set readfd;
    char buffer[1024];
    int i = 0;
    char *msg1 = "ping1";

    int port = 0;
    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) 
    {
        perror("sock error");
        return -1;
    }

    ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
    if(ret == -1) 
    {
        perror("setsockopt error");
        return 0;
    }

    addr_len = sizeof(struct sockaddr_in);

    memset((void*)&broadcast_addr, 0, addr_len);
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_addr.sin_port = htons(port);

    for(i = 0; i < 3; i++) 
    {
        ret = sendto(sock, (char *)msg1, strlen(msg1)+1, 0, (struct sockaddr*)&broadcast_addr, (socklen_t)addr_len);

        FD_ZERO(&readfd);
        FD_SET(sock, &readfd);

        /* ret = select(sock + 1, &readfd, NULL, NULL, NULL); */                /* for next exercies */

/*         if(ret > 0) 
        { */
        if(FD_ISSET(sock, &readfd))
        {
            count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&server_addr, (socklen_t *)&addr_len);
            printf("\trecvmsg is %s\n", buffer);
            
            if (strstr(buffer, "Pong")) 
            {
                printf("\tfound server IP is %d, Port is %d\n", inet_ntoa(&(server_addr.sin_addr)),htons(server_addr.sin_port));
            }
        }
/*         } */
    }

    return 0;
}