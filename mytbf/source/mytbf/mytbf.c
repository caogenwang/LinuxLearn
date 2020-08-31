#include <mytbf.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
struct mytbf_s* job[MYTBF_MAX];
static bool inited = true;
static sighandler_t alarm_handler_save;
struct mytbf_s
{
    int cps;
    int burst;
    int token;
    int pos;
};
static void do_alrm(int s)
{
    alarm(1);
    for (int i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
}

static int get_free_pos(void)
{
    for (int i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

static void module_unload()
{
    signal(SIGALRM,alarm_handler_save);
    alarm(0);
    for (int i = 0; i < MYTBF_MAX; i++)
        free(job[i]);

}

static void module_load()
{
    alarm_handler_save = signal(SIGALRM,do_alrm);
    alarm(1);
    atexit(module_unload);
}


mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_s *me = NULL;
    if (inited)
    {
        module_load();
        inited = false;
    }
    
    int pos = get_free_pos();
    if (pos < 0)
        return NULL;
    
    me = (struct mytbf_s*)malloc(sizeof(*me));
    if (me == NULL)
        return me;
    me->token = 0;
    me->cps = cps;
    me->burst = burst;
    me->pos = pos;
    job[pos] = me;
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
    while(me->token <= 0)
        pause();
    int n = min(me->token,size);
    me->token -= n;
    return n;
}

int mytbf_returnToken(mytbf_t* ptr,int size)
{
    struct mytbf_s *me = ptr;
    if(size <= 0)
        return -EINVAL;
    me->token += size;
    if (me->token > me->burst)
        me->token = me->burst;
    return size;
}

int mytbf_destory(mytbf_t* ptr)
{
    struct mytbf_s *me = ptr;
    job[me->pos] = NULL;
    free(ptr);
    return 0;
}

