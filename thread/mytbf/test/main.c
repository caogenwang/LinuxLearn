#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <mytbf.h>
#include <pthread.h>

int main(int argc,char**argv)
{
    char buf[1024] = {0};
    void* tbf;
    int cps = 10;
    int burst = 20;
    if (argc < 2)
    {
        fprintf(stderr,"Usage....\n");
        exit(1);
    }
    int fd = open(argv[1],O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr,"file open()");
        exit(1);
    }

     int dfd = open("/Users/yangsen/work/LinuxLearn/thread/mytbf/copy.txt",O_WRONLY|O_CREAT|O_TRUNC,0600);
     if (dfd<0)
     {
         fprintf(stderr,"file open()");
         exit(1);
     }
    dfd = 1;
    tbf = mytbf_init(cps,burst);
    if (tbf == NULL)
    {
        fprintf(stderr,"tbf init");
        exit(1);
    }
    
    int len = 0;
    int ret = 0;
    int size = 10;
    while (1)
    {
        int token = mytbf_fetchToken(tbf,size);
        printf("Get token is %d\n",token);
        len = read(fd,buf,token);
        if(len < 0)
        {
            perror("read()");
            break;
        }
        if (len == 0)
        {
            break;
        }
        else
        {
            while (len > 0)
            {
                ret = write(dfd,buf,len);
                if (ret < 0)
                {
                    exit(1);
                }
                mytbf_returnToken(tbf,token - ret);
                len -= ret;
            }
        }
        
        memset(buf, 0x0, 1024);
    }
    mytbf_destory(tbf);
    close(fd);
    exit(0);
}
