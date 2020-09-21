#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *func_push(void*p)
{
    puts(p);
}

static void *func(void*p)
{
    puts("Thread is working\n");

    pthread_cleanup_push(func_push,"clean 1");
    pthread_cleanup_push(func_push,"clean 2");
    pthread_cleanup_push(func_push,"clean 3");

    puts("push over!\n");

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    puts("Begin\n");

    int err = pthread_create(&tid,NULL,func,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
	exit(1);
    }
    
    pthread_join(tid,NULL);
    puts("End\n");
    exit(0);
}
