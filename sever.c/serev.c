#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_conf.h"
#include "../include/proto.h"
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <net/if.h>

/*
-M
-P
-F
-I
-D
-H
*/

struct server_conf_st server_conf={
    .rcvport = DEFAULT_PORT;
    .mgroup = DEFAULT_GROUP;
    .meidia_dir = DEFAULT_MEDIADIR;
    .runmode = RUN_DAEMON;
    .ifname = DEFAULT_IF;
}

static void  printhelp()
{
   // printf("-P --port     指定接收端\n-M --mgroup   指定多播组 \n-p --player   指定播放器命令行\n-H --help    显示帮助");
}

static void daemon_exit(int s)
{

    closelog();
    exit(0);
}
static void daemonize()
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        return -1;
    }
    if(pid > 0)
    {
        exit(0)
    }
    int fd = open("/dev/null",O_RDWR);
    if (fd < 0)
    {
        perror("open()2");
        return -2;
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if (fd > 2)
    {
        close(fd);
    }
    
    setsid();
    chdir("/")
    unmask(0);
    return 0;
}

static int socket_init()
{
    int sd;
    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd < 0)
    {
        perror("socket");
        exit(1);
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET,server_conf.mgroup,&mreq.imr_multiaddr);
    /*if error*/
    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("ens33");
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_IF,&mreq,sizeof(mreq))<0)
    {
	    perror("setsockopt(IP_MULTICAST_IF)");
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

}

int main(int argc,char *argv[])
{

    struct sigaction sa;
    sa.handler = daemon_exit;
    sigemptyset(&sa.mask);
    sigaddset(&sa.mask,SIGQUIT);
    sigaddset(&sa.mask,SIGINT);
    sigaddset(&sa.mask,SIGTERM);

    sigaction(SIGTERM,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);
    sigactioin(SIGQUIT,&sa,NULL);


    openlog("netradio",LOG_PID|LOG_PERROR,LOG_DAEMON);

    struct option argarr[] = {{"port",1,NULL,'P'},
			      {"mgroup",1,NULL,'M'},{"player",1,NULL,"p"},{"help",1,NULL,'H'},{NULL,0,NULL,0}
   		     };


    int c;
    int index;
    while(1){

        c = getopt(argc,argv,"M:P:F:I:D:H",argarr,&index);
        if(c < 0)
            break;
            switch(c)
            {
                case 'M':
                    server_conf.rcvport = optarg;
                    break;
                case 'M':
                    server_conf.mgroup = optarg;
                        break;
                case 'F':
                    server_conf.runmode = RUN_FOREGROUD;
                    break;
                case 'I':
                    server_conf.ifname = optarg;
                    break;
                case 'D':
                    server_conf.media_dir = optarg;
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



    /*shou hu jin cheng*/
    if(server_conf.runmode == RUN_DAEMON)
    {
        int ret = daemonize();
        if(ret == -1)
        {
            syslog(LOG_ERR,"fork():%s\n",strerror(errno));
        }
    }
    else if(server_conf.runmode == RUN_FOREGROUD)
    {
        /*do something*/
    }
    else
    {
        perror("run mode is error");
        exit(1);
    }
    
    socket_init();
   
   struct mlib_listentry_st *list;
   int list_size;
   int err;

    mlib_getchnlist(list,list_size);
    

    while (1)
    {
        pause();
    }
    closelog();
    exit(0);
}