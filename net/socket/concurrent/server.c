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

#define PROCESSNUM 4
static void server_job(int cfd)
{
    /*do something*/
}
static void server_loop (int tfd)
{
    int cfd;
    struct sockaddr_in raddr;
    socklen_t clie_addr_len;
    clie_addr_len = sizeof(clie_addr_len);
    char  clie_IP[BUFSIZ];

    while (1)
    {
        cfd = accept(tfd,(struct sockaddr *)&raddr, &clie_addr_len);
        if (cfd < 0)
        {
            perror("accept");
            exit(1);
        }

        printf("client IP: %s  port:%d\n", 
            inet_ntop(AF_INET, &raddr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(raddr.sin_port));
        server_job(cfd);
        close(cfd);
    }
    
}

int main()
{
    int tfd;
    struct sockaddr_in laddr;
    tfd = socket(AF_INET, SOCK_STREAM,0);
    if (tfd < 0)
    {
        perror("socket");
        exit(1);
    }

    int opt_val = 1;
    setsockopt(tfd,SOL_SOCKET,SO_REUSEADDR,&opt_val,sizeof(opt_val));

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(tfd,(struct sockaddr *)&laddr,sizeof(laddr))<0)
    {
        perror("bind");
        exit(1);
    }
    
    if (listen(tfd,200) < 0)
    {
        perror("listen");
        exit(1);
    }
    printf("listen:%d\n",tfd);
    pid_t pid;
    for (int i = 0; i < PROCESSNUM; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            server_loop(tfd);
            exit(0);
        }   
    }
    
    for (int i = 0; i < PROCESSNUM; i++)
    {
        wait(NULL);
    }

    close(tfd);
    exit(0);

}