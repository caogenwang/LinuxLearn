#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

static int  semid;
static int num = 0;


static void P(void)
{
    struct sembuf op;
    op.sem_op = -1;
    op.sem_num = 0;
    op.sem_flg = 0;

    while(semop(semid,&op,1) < 0)
    {
        if (errno == !EINTR || errno != EAGAIN)
        {
            perror("semop");
            exit(1);
        }
    }
}

static void V(void)
{
    struct sembuf op;
    op.sem_op = 1;
    op.sem_num = 0;
    op.sem_flg = 0;

    if(semop(semid,&op,1) < 0)
    {
        perror("semop");
        exit(1);
    }
}

static void *add(void *p)
{
    P();
    num ++;
    V();
}

int main()
{
    semid = semget(IPC_PRIVATE,1,0600);
    pthread_t tids[20];
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    int ret = semctl(semid,0,SETVAL,1) ;
    if (ret < 0)
    {
        perror("semctl");
        exit(1);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&tids[i],NULL,add,NULL);
    }
    
    for (int i = 0; i < 10; i++)
    {
        pthread_join(tids[i],NULL);
    }
    printf("%d\n",num);
    semctl(semid,1,IPC_RMID);
    exit(0);
}