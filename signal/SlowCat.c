#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CPS 10
#define BUFSIZE 1024
#define BURST 100

int main(int argc,char**argv)
{
    int sfd,dfd;
    char buf[1024];
    if (argc < 2)
    {
        exit(1);
    }
    sfd = open(argv[1],O_RDONLY);
    if(sfd < 0)
    {
        exit(1);
    }
    int len = 0;
    int ret = 0;
    int pos = 0;
    while (1)
    {
        len = read(sfd,buf,BUFSIZE);
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
                ret = write(0,buf+pos,len);
                if (ret < 0)
                {
                    exit(1);
                } 
                pos += ret;
                len -= ret;
            }
        }
    }
    close(sfd);
    
}