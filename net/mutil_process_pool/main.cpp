
#include "processpool.h"
#include "cgi_server.h"

#define SERVERCGIPORT 666

int main(int argc,char *argv[])
{
    int listenfd;
    listenfd = socket(AF_INET,SOCK_STREAM,0);   
    if (listenfd < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERCGIPORT);
    if (bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd,5)<0)
    {
        perror("listen");
        exit(1);
    }
    
    ProcessPool<cgi_conn> * processpoll = ProcessPool<cgi_conn>::create(listenfd,5);
    //子进程怎么执行？
    if (processpoll)
    {
        processpoll->run();
        delete processpoll;
    }
    close(listenfd);
    return 0;
}
