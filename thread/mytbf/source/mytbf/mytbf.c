#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <mytbf.h>
struct mytbf_s* job[MYTBF_MAX];
static bool inited = true;
pthread_t tid_alrm;
pthread_once_t init_once;
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
struct mytbf_s
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mutext;
    pthread_cond_t mucond;
};
static void *thr_alrm(int *s)
{
    printf("add token\n");
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&mut_job);
        for (int i = 0; i < MYTBF_MAX; i++)
        {
            if (job[i] != NULL)
            {
                pthread_mutex_lock(&job[i]->mutext);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_cond_broadcast(&job[i]->mucond);
                pthread_mutex_unlock(&job[i]->mutext);
                printf("after add,job[%d]'s token is %d\n",i,job[i]->token);
            }
            
        }
        pthread_mutex_unlock(&mut_job);
    }
}

static int get_free_pos_unlocked(void)
{
    for (int i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

static void module_unload()
{
    pthread_cancel(tid_alrm);
    pthread_join(tid_alrm,NULL);
    
    for (int i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            mytbf_destory(job[i]);
        }
        
    }
    printf("module_unload\n");
    pthread_mutex_destroy(&mut_job);

}

static void module_load()
{
    printf("module_load\n");
    int err = pthread_create(&tid_alrm,NULL,thr_alrm,NULL);
    if(err){
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
        exit(1);
    }
    atexit(module_unload);
}


mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_s *me = NULL;
    pthread_once(&init_once,module_load);

    me = (struct mytbf_s*)malloc(sizeof(*me));
    if (me == NULL)
        return me;
    me->token = 0;
    me->cps = cps;
    me->burst = burst;
    pthread_mutex_init(&me->mutext,NULL);
    pthread_cond_init(&me->mucond,NULL);

    pthread_mutex_lock(&mut_job);
    int pos = get_free_pos_unlocked();
    if (pos < 0)
    {
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }
    me->pos = pos;
    job[pos] = me;
    pthread_mutex_unlock(&mut_job);
    return me;
}

static int min(int a,int b)
{
    return a<b?a:b;
}

int mytbf_fetchToken(mytbf_t* ptr,int size)
{
    struct mytbf_s *me = ptr;
    if (size <= 0)
        return -EINVAL;
    
    pthread_mutex_lock(&me->mutext);
    printf("first token %d\n",me->token);
    while(me->token <= 0)
    {
        printf("Token is zero,wating......\n");
        pthread_cond_wait(&me->mucond,&me->mutext);
    }
    
    int n = min(me->token,size);
    me->token -= n;
    printf("left token %d\n",me->token);
    pthread_mutex_unlock(&me->mutext);

    return n;
}

int mytbf_returnToken(mytbf_t* ptr,int size)
{
    printf("return toke is %d\n",size);
    struct mytbf_s *me = ptr;
    if(size <= 0)
        return -EINVAL;
    pthread_mutex_lock(&me->mutext);
    me->token += size;
    if (me->token > me->burst)
        me->token = me->burst;
    pthread_cond_broadcast(&me->mucond);
    pthread_mutex_unlock(&me->mutext);
    return size;
}

int mytbf_destory(mytbf_t* ptr)
{
    struct mytbf_s *me = ptr;
    pthread_mutex_lock(&mut_job);
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);
    pthread_mutex_destroy(&me->mutext);
    pthread_cond_destroy(&me->mucond);
    free(ptr);
    return 0;
}

