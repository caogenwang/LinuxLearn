#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
static volatile int loop = 1;

static void alrm_handler(int sig)
{
    loop = 0;
}

int main()
{
    int count = 0;
    alarm(5);
    signal(SIGALRM,alrm_handler);
    while(loop)
        count++;
    printf("%lld\n",count);
    exit(0);
}
