#include <relay.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/epoll.h>
#define BUFSIZE 1024
enum 
{
    STATE_RUNNING=1,
    STATE_CANCLED,
    STATE_OVER
};

enum
{
    STATE_R=1,
    STATE_W,
    STATE_AUTO,
    STATE_Ex,
    STATE_T
};

struct rel_fsm_st
{
    int state;
    int sfd;
    int dfd;
    int len;
    int pos;
    char *errstr;
    char buf[BUFSIZE];
    int64_t count;
};

struct rel_job_st
{
    int job_state;
    int fd1;
    int fd2;
    struct rel_fsm_st fsm12,fsm21;
    int pos;
    int fd1_save;
    int fd2_save;
    char *reason;
    int64_t count1,count2;
};
static void fsm_driver(struct rel_fsm_st*fsm);

static void fsm_driver_select(int fd1,int fd2)
{
    //布置监视任务
    struct rel_fsm_st fsm12,fsm21;
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;
    int efd = epoll_create(5);
    if (efd < 0)
    {

    }
    struct epoll_event ev;
    ev.events = 0;
    ev.data.fd = fd1;
    epoll_ctl(efd,EPOLL_CTL_ADD,fd1,&ev);

    ev.events = 0;
    ev.data.fd = fd2;
    epoll_ctl(efd,EPOLL_CTL_ADD,fd2,&ev);

    while (fsm12.state =! STATE_T ||fsm21.state == STATE_T)
    {
        ev.events = 0;
        ev.data.fd = fd1;
        

        if (fsm12.state == STATE_R)
            ev.events |= EPOLLIN;
        if (fsm21.state == STATE_W)
            ev.events |= EPOLLOUT;
        epoll_ctl(efd,EPOLL_CTL_MOD,fd1,&ev);

        ev.data.fd = fd1;
        ev.events = 0;
        if (fsm12.state == STATE_W);
            ev.events |= EPOLLOUT;
        if (fsm21.state == STATE_R)
            ev.events |= EPOLLIN;
        epoll_ctl(efd,EPOLL_CTL_MOD,fd2,&ev);

        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            while(epoll_wait(efd,&ev,1,-1)<0)
            {
                if (errno == EINTR)
                    continue;
                exit(1);
            }
        }

        if ((ev.data.fd == fd1 && ev.events & EPOLLIN)|| ((ev.data.fd == fd2 && ev.events & EPOLLOUT) || fsm12.state > STATE_AUTO)
            fsm_driver(&fsm12);
        if ((ev.data.fd == fd2 && ev.events & EPOLLIN)|| ((ev.data.fd == fd1 && ev.events & EPOLLOUT) || fsm21.state > STATE_AUTO)
            fsm_driver(&fsm21);
        
    }
    close(efd);
}