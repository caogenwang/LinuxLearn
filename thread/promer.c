#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define LEFT 0
#define RIGHT 200
#define THREAD_NUM (RIGHT-LEFT+1)
struct argment{
    int i;
};
static void *thr_prime(void *p);
int main()
{
    int i =0;
    struct argment *p;
    pthread_t tid[THREAD_NUM];
    for(i=LEFT;i<RIGHT;i++)
    {
	p = malloc(sizeof(struct argment));
	if(p==NULL)
	{
	    exit(1);
	}
	p->i = i;
	int err = pthread_create(&tid[i-LEFT],NULL,thr_prime,(void*)p);
	if(err)
	{
	    fprintf(stderr,"pthread_create():%s\n",strerror(err));
	    exit(1);
	}
    }
    int j=0;
    struct argment *ptr;
    for(j=LEFT;j<RIGHT;j++)
    {
	pthread_join(tid[j-LEFT],&ptr);
	free(ptr);
    }
}

static void *thr_prime(void *p)
{
    struct argment *arg = (struct argment*)p;
    int mark=1;
    int j;
    for(j=2;j<arg->i/2;j++)
    {
	if(arg->i % j == 0)
	{
	   mark=0;
	   break;
	}
    }
    if(mark)
        printf("%d is primer\n",arg->i);
     pthread_exit(p);
}
