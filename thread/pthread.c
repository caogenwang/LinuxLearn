#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *func(void*p)
{
    puts("Thread is working\n");
    return NULL;
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
