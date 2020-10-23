#include <relay.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <poll.h>
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
    struct pollfd pfd[2];
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    pfd[0].fd = fd1;
    pfd[1].fd = fd2;

    while (fsm12.state =! STATE_T ||fsm21.state == STATE_T)
    {
        pfd[0].events = 0;
        if (fsm12.state == STATE_R)
            pfd[0].events |= POLLIN;
        if (fsm21.state == STATE_W)
            pfd[0].events |= POLLOUT;

        pfd[1].events = 0;
        if (fsm12.state == STATE_W)
            pfd[1].events |= POLLOUT;
        if (fsm21.state == STATE_R)
            pfd[1].events |= POLLIN;


        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            while(poll(pfd,2,-1) < 0)
            {
                if (errno == EINTR)
                    continue;
                exit(1);
            }
        }
        if (pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT || fsm12.state > STATE_AUTO)
            fsm_driver(&fsm12);
        if (pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT || fsm21.state > STATE_AUTO)
            fsm_driver(&fsm21);
        
    }
}