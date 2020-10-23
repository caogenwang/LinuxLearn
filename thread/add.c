#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 20

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

static void *thr_add(void *p)
{
    FILE *fp;
    char linebuf[1024];
    fp = fopen("test.txt","r+");
    pthread_mutex_lock(&mutex);
    fgets(linebuf,1024,fp);
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d\n",atoi(linebuf)+1); 
    fclose(fp);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);


}

int main()
{
    int i =0;
    struct argment *p;
    pthread_t tid[THREAD_NUM];
    for(i=0;i<THREAD_NUM;i++)
    {
	int err = pthread_create(&tid[i],NULL,thr_add,NULL);
	if(err)
	{
	    fprintf(stderr,"pthread_create():%s\n",strerror(err));
	    exit(1);
	}
    }
    int j=0;
    for(j=0;j<THREAD_NUM;j++)
    {
	    pthread_join(tid[j],NULL);
    }
}

