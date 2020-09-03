#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "proto.h"

int main()
{
    int ufd;
    struct sockaddr_in laddr,raddr;
    ufd = socket(AF_INET, SOCK_DGRAM,0);
    if (ufd < 0)
    {
        perror("socket");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(ufd,(struct sockaddr *)&laddr,sizeof(laddr))<0)
    {
        perror("bind");
        exit(1);
    }
    struct msg_st rbuf;
    socklen_t raddr_len;
    char clie_IP[BUFSIZ];
    raddr_len = sizeof(raddr);
    while (1)
    {
        recvfrom(ufd,&rbuf,sizeof(rbuf),0,(struct sockaddr *)&raddr,&raddr_len);
        printf("MSG FROM IP: %s  port:%d\n", 
            inet_ntop(AF_INET, &raddr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(raddr.sin_port));
        printf("NAME:%s\n",rbuf.name);
        printf("math:%d\n",rbuf.math);
        printf("chinese:%d\n",rbuf.chinese);
    }
    

    close(ufd);
    exit(0);

}