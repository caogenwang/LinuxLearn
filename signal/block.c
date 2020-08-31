#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void int_handler(int i)
{
    write(1,"%%%",3);
}

int main()
{
    int i;
    sigset_t set,oset,saveset;
    signal(SIGINT,int_handler);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_UNBLOCK,&set,&saveset);
    for (int i = 0; i < 1000; i++)
    {
        sigprocmask(SIG_BLOCK,&set,&oset);
        for (int j = 0; j < 5; j++)
        {
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        sigprocmask(SIG_SETMASK,&oset,NULL);
    }
    sigprocmask(SIG_UNBLOCK,&saveset,NULL);
    exit(0);
}