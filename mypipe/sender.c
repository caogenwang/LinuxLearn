#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include "proto.h"

int main()
{
    key_t key;
    int msgid;
    struct msg_st sendbuf;
    key = ftok(KEYPATH,KEYPROJ);
    if (key < 0)
    {
        perror("ftok()");
        exit(1);
    }
    
    msgid = msgget(key,0);
    if (msgid < 0)
    {
        perror("meg_recv()");
        exit(1);
    }
    sendbuf.mtype = 1;
    strcpy(sendbuf.name,"Alen");
    sendbuf.chinese = 100;
    sendbuf.math = 99;
    int ret = msgsnd(msgid,&sendbuf,sizeof(sendbuf)-sizeof(long),0);
    if (ret < 0)
    {
        perror("msgsnd()");
        exit(1);
    }
    puts("OK");
    exit(0);
}