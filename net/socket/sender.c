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
    
    ufd = socket(AF_INET, SOCK_DGRAM,0);
    if (ufd < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,"0.0.0.0",&raddr.sin_addr);

    struct msg_st sbuf;
    sbuf.chinese = 90;
    sbuf.math = 89;
    strcpy(sbuf.name,"xiaoming");
    while (1)
    {
        if(sendto(ufd,&sbuf,sizeof(sbuf),0,(struct sockaddr *)&raddr,sizeof(raddr))<0)
        {
            perror("sendto");
            exit(1);
        }
        sleep(1);
    }
    close(ufd);

    exit(0);
}