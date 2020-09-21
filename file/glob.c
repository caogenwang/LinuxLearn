#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char**argv)
{
    DIR dp = NULL;
    dp =  opendir();	
    printf("%d\n",agrc);
    return 0;
}
