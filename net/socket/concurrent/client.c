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

int main(int argc,char*argv[])
{
    
    int tfd;
    tfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tfd < 0)
    {
        perror("socket");
        exit(1);
    }
    tfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in raddr;
    bzero(&raddr,sizeof(raddr));
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"127.0.0.1",&raddr.sin_addr);
    if (connect(tfd,(struct sockaddr *)&raddr,sizeof(raddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    while (1)
    {
        sleep(1);
    }
    
    
    close(tfd);

    exit(0);
}