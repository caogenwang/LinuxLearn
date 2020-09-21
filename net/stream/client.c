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
    int tfd;
    
    tfd = socket(AF_INET, SOCK_STREAM,0);
    if (tfd < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&raddr.sin_addr);
    int ret = connect(tfd,(struct sockaddr *)&raddr,sizeof(raddr));
    if(ret < 0)
    {
	perror("connect");
	exit(1);
    }
    long long stamp;
    FILE *fp = fdopen(tfd,"r+");
    if(fscanf(fp,FMT_STAMP,&stamp)<0)
	fprintf(stderr,"fscan");
    else
	fprintf(stdout,"%lld\n",stamp);
 

   
   fclose(fp);

    exit(0);
}
