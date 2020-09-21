#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "client.h"
#include "../include/proto.h"
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <net/if.h>
struct client_conf_st client_conf = {\
    .rcvport = DEFAULT_RCVPORT,\
    .mgroup = DEFAULT_MGROUP,\
    .player_cmd = DEFAULT_PLAYERCMD};
static void  printhelp()
{
   // printf("-P --port     指定接收端\n-M --mgroup   指定多播组 \n-p --player   指定播放器命令行\n-H --help    显示帮助");
}
static writen(int fd,const char*buf,size_t len)
{
    int ret = 0;
    int pos = 0;
    while(1)
    {
	ret = write(fd,buf+pos,len);
	if(ret < 0)
	{
	    if(errno == EINTR)
		continue;
	    perror("write()");
	    exit(1);
	}
	len -= ret;
	pos += ret;
    }	
    return pos;
}

int main(int argc,char *argv)
{
    struct option argarr[] = {{"port",1,NULL,'P'},
			      {"mgroup",1,NULL,'M'},{"player",1,NULL,"p"},{"help",1,NULL,'H'},{NULL,0,NULL,0}
   		     };
   
    
    int c;
    int index;
    while(1){

	c = getopt_long(argc,argv,"P:M:p:H",argarr,&index);
	if(c < 0)
	    break;
	switch(c)
	{
	    case 'P':
		client_conf.rcvport = optarg;
		break;
	    case 'M':
		client_conf.mgroup = optarg;
	        break;
	    case 'p':
		client_conf.player_cmd = optarg;
		break;
	    case 'H':
		printhelp();
		exit(0);
		break;
	    default:
		abort();
		break;
	}	
		
    }

    int sd;
    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd < 0)
    {
	perror("socket()");
	exit(1);
    }
    struct ip_mreqn mreq;
    inet_pton(AF_INET,client_conf.mgroup,&mreq.imr_multiaddr);
    /*if error*/
    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("ens33");
    
    if(setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0)
    {
	perror("setsockopt()1");
        exit(1);
    }
    int val = 1;
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_LOOP,&val,sizeof(val))<0)
    {
         perror("setsockopt()2");
         exit(1);
    }     
    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(sd,(void *)&laddr,sizeof(laddr))<0)
    {
	perror("bind()");
        exit(1);
    }
	
    int pd[2];
    if(pipe(pd)<0)
    {
	perror("pipe()");
	exit(1);	
    }


    pid_t pid = fork();
    if(pid < 0)
    {
	perror("f0rk()");
	exit(1);
    }
    
    socklen_t serveraddrLen;
    struct sockaddr_in serveraddr;
    int chooseid;
    if(pid == 0)
    {
	//解码器
	close(sd);
        close(pd[1]);
        dup2(pd[0],0);
    	if(pd[0] > 0)
	    close(pd[0]);
        execl("/bin/sh","sh","-c",client_conf.player_cmd,NULL);
        perror("execl()");
        exit(1);
    }
    else if(pid > 0)
    {
	//写管道,收节目单，选择频道，收频道包，
	struct msg_list_st *msg_list;
	msg_list = malloc(sizeof(MSG_LIST_MAX));
	if(msg_list == NULL)
	{
	    perror("malloc");
	    exit(1);
	}
	while(1){
       	    int len = recvfrom(sd,msg_list,MSG_LIST_MAX,0,(void *)&serveraddr,&serveraddrLen);	
    	    if(len < 0)
	    {   
	          perror("msg is too small"); 
		  continue;
            }
            if(msg_list->chnid != LISTCHNID)
	    {
		perror("chnid is not match");
		continue;
            }
            break;
	}
	//print list
	struct msg_listentry_st *pos;
	for(pos=msg_list->entry;pos < (((char*)msg_list) + pos->len);pos = (void *)(((char*)pos) + ntohs(pos->len)))
	{
	      printf("chnid:%d:%s\n",pos->chnid,pos->desc);  	
	}
	int ret = 0;
	do{
	    ret = scanf("%d",&chooseid);
            if(ret != 1)
		exit(1);
	}while(1);
	struct msg_channel_st *msg_channel;
	msg_channel = malloc(MSG_CHANNEL_MAX);
	if(msg_channel == NULL)
	{
	    
	}
	struct sockaddr_in raddr;
	socklen_t raddr_len;
	int len = 0;
	while(1){
			
		recvfrom(sd,msg_channel,MSG_CHANNEL_MAX,0,(void*)&raddr,&raddr_len);
		if(raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr ||  raddr.sin_port != serveraddr.sin_port)
		{
			perror("addr is not match");
			continue;			
		}
		if(len < sizeof(struct msg_channel_st))
		{
			perror("too small");
			continue;
		}
		if(msg_channel ->chnid == chooseid)
		{
			if(writen(pd[1],msg_channel->data,len-sizeof(chnid_t)<0))
			{
				exit(1);			     
			}
		}
	}
    }
    exit(0);
}

