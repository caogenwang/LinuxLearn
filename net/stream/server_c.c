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
static void server_job(int fd)
{
    char buf[1024];
    int len = sprintf(buf,FMT_STAMP,(long long)time(NULL));
    if(send(fd,buf,len,0) < 0)
    {
	perror("send()");
	exit(1);
    }
}
int main()
{
    int tfd;
    struct sockaddr_in laddr,raddr;
    tfd = socket(AF_INET, SOCK_STREAM,0);
    if (tfd < 0)
    {
        perror("socket");
        exit(1);
    }

   // setsockopt(tfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));    

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(tfd,(struct sockaddr *)&laddr,sizeof(laddr))<0)
    {
        perror("bind");
        exit(1);
    }
    if(listen(tfd,5) < 0)
    {
        perror("listen");
        exit(1);	
    }
    socklen_t raddr_len;
    raddr_len = sizeof(raddr_len);
    char clie_IP[BUFSIZ];
    pid_t pid;
    while(1)
    {
        int afd = accept(tfd,(struct sockaddr*)&raddr,&raddr_len);
	if(afd < 0)
	{
	    perror("accept()");
	    exit(1);
	}
	

        pid = fork();
	if(pid < 0)
	{
	    perror("fork()");
	    exit(1);	
	}
        if(pid == 0)
	{
	    close(tfd);
            printf("Client: %s  port:%d\n",
                 inet_ntop(AF_INET, &raddr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(raddr.sin_port));
	    server_job(afd);
	    close(afd);
	    exit(0);
         }
	close(afd); 
    }

    close(tfd);
    exit(0);

}
