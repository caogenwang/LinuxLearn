#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include "proto.h"

#define PROCESSNUM 4
#define MINPOOL 5
#define MAXPOOL 10
#define CLIENTMAX 20
#define SIG_NOTIFY SIGUSR1
static struct server_st *serverpool;
static int idle_count = 0;
static int busy_count = 0;


enum
{
    STATE_IDEL=1,
    STATE_BUSY
};

struct server_st
{
    pid_t pid;
    int state;
    // int reuse;
};


static void server_job(int pos,int tfd)
{
    int ppid;
    ppid = getppid();
    int cfd;
    struct sockaddr_in raddr;
    socklen_t clie_addr_len;
    clie_addr_len = sizeof(clie_addr_len);
    char  clie_IP[BUFSIZ];
    
    while (1)
    {
        printf("wait for accept\n");
        serverpool[pos].pid = STATE_IDEL;
        cfd = accept(tfd,(struct sockaddr *)&raddr, &clie_addr_len);
        if (cfd < 0)
        {
            if (errno != EINTR || errno != EAGAIN)
            {
                perror("accept()");
                exit(1);
            }
        }
        serverpool[pos].state = STATE_BUSY;
        kill(ppid,SIG_NOTIFY);//给父进程发送一个信号，这是用户自定义的一个信号

        printf("client IP: %s  port:%d\n", 
            inet_ntop(AF_INET, &raddr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(raddr.sin_port));

        time_t stamp;
        time(&stamp);
        char linebuf[1024];
        int len = sprintf(linebuf,FMT_STAMP,(int)stamp);
        sleep(5);
        send(cfd,linebuf,len,0);
    }
    
}

static void scan_pool()
{
    int idel = 0,busy = 0;
    for (int i = 0; i < CLIENTMAX; i++)
    {
        if (serverpool[i].pid == -1)
            continue;
        if (kill(serverpool[i].pid,0) > 0)
        {
            serverpool[i].pid = -1;
            continue;
        }
        if (serverpool[i].state == STATE_IDEL)
        {
            idel++;
        }
        else if(serverpool[i].state == STATE_BUSY)
        {
            busy++;
        }
        else
        {
            fprintf(stderr,"Unknown state.\n");
            abort();
        }
    }
    printf("idel:%d,busy:%d\n",idel,busy);
}
static int  del_one_server()
{
    if (idle_count == 0)
    {
        return -1;
    }
    for (int i = 0; i < CLIENTMAX; i++)
    {
        if(serverpool[i].pid != -1 && serverpool[i].state == STATE_IDEL)
        {
            kill(serverpool[i].pid,SIGTERM);//将当前的进程杀死
            serverpool[i].pid = -1;
            idle_count --;
        }
    }
    return 0;
}

static int  add_one_server(int tfd)
{
    if (idle_count + busy_count >= CLIENTMAX)
    {
        return -1;
    }
    int slot;
    for (slot = 0; slot < CLIENTMAX; slot++)
    {
        if(serverpool[slot].pid == -1)
            break;
    }
    serverpool[slot].state = STATE_IDEL;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if (pid == 0)
    {
        server_job(slot,tfd);//子进程去工作
        exit(0);
    }
    else
    {
        serverpool[slot].pid = pid;
        idle_count ++;
    }
    return 0;
}

static void usr2_handler(int s)
{
     write(1,"*",1);
}

int main()
{
    struct sigaction sa,osa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD,&sa,&osa);//忽略子进程的信号，不会产生僵尸进程

    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY,&sa,&osa);//用户的自定义信号

    sigset_t set,oset;
    sigemptyset(&set);
    sigaddset(&set,SIG_NOTIFY);
    sigprocmask(SIG_BLOCK,&set,&oset);//在这期间不响应用户自定义的信号，也就是进行了屏蔽


    serverpool = mmap(NULL,sizeof(struct server_st)*CLIENTMAX,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
    if (serverpool == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    for (int i = 0; i < CLIENTMAX; i++)
    {
        serverpool[i].pid = -1;
    }
    
    int tfd;
    if ((tfd = socket(AF_INET, SOCK_STREAM,0))< 0)
    {
        perror("socket");
        exit(1);
    }

    int opt_val = 1;
    if(setsockopt(tfd,SOL_SOCKET,SO_REUSEADDR,&opt_val,sizeof(opt_val)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in laddr;
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
    
    while (1)
    {
        scan_pool();
        if(idle_count > MAXPOOL)
        {
            for (int i = 0; i < (idle_count - MAXPOOL); i++)
            {
                del_one_server();
            }
        }
        else if(idle_count - MINPOOL < 0)
        {
            for (int i = idle_count; i < (MINPOOL - idle_count); )
            {
                add_one_server(tfd);
            }
        }
        sigsuspend(&oset);//程序挂起，等待一个唤醒的信号
    }
    sigprocmask(SIG_SETMASK,&oset,NULL);//接触屏蔽
    close(tfd);
    exit(0);

}