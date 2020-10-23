#include <relay.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/select.h>
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
    fd_set rset,wset;
    struct rel_fsm_st fsm12,fsm21;

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_W;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;


    while (fsm12.state =! STATE_T ||fsm21.state == STATE_T)
    {
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if (fsm12.state == STATE_R)
            FD_SET(fsm12.sfd,&rset);
        if (fsm12.state == STATE_W)
            FD_SET(fsm12.dfd,&wset);

        if (fsm21.state == STATE_R)
            FD_SET(fsm21.sfd,&rset);
        if (fsm21.state == STATE_W)
            FD_SET(fsm21.dfd,&wset);
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            if(select(max(fd1,fd2),&rset,&wset,NULL,NULL) < 0)
            {
                if (errno == EINTR)
                    continue;
                exit(1);
            }

            if (FD_ISSET(fd1,&rset) || FD_ISSET(fd2,&wset))
                fsm_driver(&fsm12);
            if (FD_ISSET(fd2,&rset) || FD_ISSET(fd1,&wset))
                fsm_driver(&fsm21);

        }
        
    }
}