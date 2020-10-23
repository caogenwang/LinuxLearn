#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>
#include <relay.h>
#define TTY1 "dev/ttyp1"
#define TTY2 "dev/ttyp2"
#define TTY3 "dev/ttyp3"
#define TTY4 "dev/ttyp4"


int main()
{
    int fd1,fd2,fd3,fd4;
    fd1 = open(fd1,O_RDWR);
    if (fd1 < 0)
    {
        perror("open()1");
        exit(1);
    }
    write(1,"TTY1\n",5);

    fd2 = open(fd2,O_RDWR);
    if (fd2 < 0)
    {
        perror("open()2");
        exit(1);
    }
    write(1,"TTY2\n",5);
    int job1 = rel_addjob(fd1,fd2);
    if (job1)
    {
        fprintf(stderr,"rel_addjpb():%s\n",strerror(job1));
        exit(1);
    }
    
    fd3 = open(fd3,O_RDWR);
    if (fd3 < 0)
    {
        perror("open()3");
        exit(1);
    }
    write(1,"TTY3\n",5);

    fd4 = open(fd4,O_RDWR);
    if (fd4 < 0)
    {
        perror("open()4");
        exit(1);
    }
    write(1,"TTY4\n",5);
    int job2 = rel_addjob(fd1,fd2);
    if (job2)
    {
        fprintf(stderr,"rel_addjpb():%s\n",strerror(job2));
        exit(1);
    }

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    exit(0);

}