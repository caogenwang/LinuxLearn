#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


#define BUFSIZE 1024
int main()
{

    FILE *fp = NULL;
    time_t stamp;
    struct  tm *tm;
    int count = 0;
    char buf[BUFSIZE];
    fp = fopen("time.log","a+");

    if (fp == NULL)
    {
        perror("fopen()");
        exit(0);
    }

    while (fgets(buf,BUFSIZE,fp) != NULL)
        count++;
        

    while (1)
    {
        time(&stamp);
        tm = localtime(&stamp);
        fprintf(fp,"%-4d%d-%d-%d %d::%d::%d\n",++count,tm->tm_year,tm->tm_mon,tm->tm_mday,
                                                tm->tm_hour,tm->tm_min,tm->tm_sec);
        sleep(1);
    }
    fclose(fp);
    
    exit(0);
}