#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "proto.h"

int main()
{
    key_t key;
    int msgid;
    struct msg_st recvbuf;
    key = ftok(KEYPATH,KEYPROJ);

    msgid = msgget(key,IPC_CREAT|0600);
    if (msgid < 0)
    {
        perror("meg_recv()");
        exit(1);
    }
    while (1)
    {
        int len = msgrcv(msgid,&recvbuf,sizeof(recvbuf)-sizeof(long),0,0);
        if (len < 0)
        {
            perror("meg_recv()");
            exit(1);
        }
        printf("name:%s\n",recvbuf.name);
        printf("math:%d\n",recvbuf.math);
        printf("chinese:%d\n",recvbuf.chinese);
    }
    

    msgctl(msgid,IPC_RMID,NULL);

    exit(0);

}