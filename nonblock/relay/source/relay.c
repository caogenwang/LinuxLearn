#include <relay.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/select.h>
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
    STATE_Ex,
    STATE_T
};

#define JOB_NUM_MAX 10000
#define BUFSIZE 1024
static struct rel_job_st *jobs[JOB_NUM_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
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
static void module_unload()
{
    for (int i = 0; i < JOB_NUM_MAX; i++)
        free(jobs[i]); 
}

static void fsm_driver(struct rel_fsm_st*fsm)
{
    int ret;
    switch (fsm->state)
    {
        case STATE_R:
            fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
            if (fsm->len == 0)
            {
                fsm->state = STATE_T;
            }
            else if(fsm->len < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_R;
                }
                else
                {
                    fsm->errstr = "read";
                    fsm->state = STATE_Ex;
                }
            }
            else
            {
                fsm->pos = 0;
                fsm->state = STATE_W;
            }
            break;
        case STATE_W:
            ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
            if (ret < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_W;
                }
                else
                {
                    fsm->errstr = "write";
                    fsm->state = STATE_Ex;
                }
            }
            else 
            {
                fsm->pos += ret;
                fsm->len -= ret;
                if (fsm->len > 0)
                {
                    fsm->state = STATE_W;
                }
                else
                {
                    fsm->state = STATE_R;
                }
            }
            break;
        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            /*do something*/
            break;
        default:
            abort();
            break;
    }
}

static int max(int a,int b)
{
    if (a > b)
        return a;
    else
        return b;
}

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

static void *thr_relayer(void *s)
{
    while (1)
    {
        pthread_mutex_lock(&mut_job);
        for (int i = 0; i < JOB_NUM_MAX; i++)
        {
            if (jobs[i] != NULL && jobs[i]->job_state == STATE_RUNNING)
            { 
                    fsm_driver(&jobs[i]->fsm12);
                    fsm_driver(&jobs[i]->fsm21);
                if (jobs[i]->fsm12.state == STATE_T && \
                        jobs[i]->fsm21.state == STATE_T)
                    jobs[i]->job_state = STATE_OVER;
            }
            
        }
        pthread_mutex_unlock(&mut_job);
    }
}

static void module_load()
{
    pthread_t tid;
    int err = pthread_create(&tid,NULL,thr_relayer,NULL);
    if (err < 0)
    {
         fprintf(stderr,"module_load():%s\n",strerror(err));
    }
    
    atexit(module_unload);
}

static get_job_pos_unlocked()
{
    for (int i = 0; i < JOB_NUM_MAX; i++)
    {
        if (NULL == jobs[i])
        {
            return i;
        }
    }
    return -1;
}

int rel_addjob(int fd1,int fd2)
{
    struct rel_job_st *me;
    pthread_once(&init_once,module_load);
    me = malloc(sizeof(*me));
    if (NULL == me)
    {
        return -ENOMEM;
    }
    me->fd1 = fd1;
    me->fd2 = fd2;
    me->job_state = STATE_RUNNING;

    me->fd1_save = fcntl(me->fd1,F_GETFL);
    fcntl(me->fd1,me->fd1_save |O_NONBLOCK);
    me->fd2_save = fcntl(me->fd2,F_GETFL);
    fcntl(me->fd2,me->fd2_save |O_NONBLOCK);

    me->fsm12.sfd = me->fd1;
    me->fsm12.dfd = me->fd2;
    me->fsm12.state = STATE_R;

    me->fsm21.sfd = me->fd2;
    me->fsm21.dfd = me->fd1;
    me->fsm21.state = STATE_R;
    pthread_mutex_lock(&mut_job);
    int pos = get_job_pos_unlocked();
    if (pos < 0)
    {
        fcntl(me->fd1,me->fd1_save);
        fcntl(me->fd2,me->fd2_save);
        pthread_mutex_unlock(&mut_job);
        free(me);
        return -ENOSPC;
    }
    me->pos = pos;
    jobs[pos] = me;
    pthread_mutex_unlock(&mut_job);
    return 0;
}

void rel_destroyjob(struct rel_job_st*ptr)
{
    struct rel_job_st *me = ptr;
    jobs[me->pos] = NULL;
    free(ptr);
}