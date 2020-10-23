#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#define THREAD_NUM 4
static int num = 0;
static pthread_mutex_t mutex_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static int next_num(int num)
{
    num = num + 1;
    if (num>=4)
    {
        num = 0;
    }
    return num;
}

static void *thr_num(void *p)
{
    int n = 0;
    printf("*****%d\n",*(int*)p);
    int c = 'a' + n;
    while (1)
    {
        pthread_mutex_lock(&mutex_num);
        while (num != n)
            pthread_cond_wait(&cond_num,&mutex_num);
        write(1,&c,1);
        num = next_num(num);
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mutex_num);
    }
    
}

int main()
{
    pthread_t tids[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        int err = pthread_create(&tids[i],NULL,thr_num,(void*)i);
    }
    alarm(5);
    sleep(500);
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(tids[i],NULL);
    }
    
    pthread_mutex_destroy(&mutex_num);
    pthread_cond_destroy(&cond_num);
    exit(0);
}