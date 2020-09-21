#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>


int main(void)
{
    alarm(5);
    while (1);
    exit(0);
}
