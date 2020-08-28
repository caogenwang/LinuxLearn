#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char**argv)
{
    int sfd,dfd;
    char buf[1024];
    if (argc < 3)
    {
        exit(1);
    }
    sfd = open(argv[1],O_RDONLY);
    if(sfd < 0)
    {
        exit(1);
    }
    dfd = open(argv[2],O_WRONLY|O_CREAT,O_TRUNC,0600);
    if(dfd < 0)
    {
        close(sfd);
        exit(1);
    }
    int len = 0;
    int ret = 0;
    while (1)
    {
        len = read(sfd,buf,1024);
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
            ret = write(dfd,buf,len);
            if (ret < 0)
            {
                break;
            }   
        }
    }
    

    close(sfd);
    close(dfd);
    
}