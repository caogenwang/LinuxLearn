#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int flen(const char*fname)
{
    struct stat statres;
    if (stat(fname,&statres) < 0)
    {
        perror("stat()");
        exit(1);    
    }   
    
    return statres.st_size;
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

    flen(argv[1]);
    return 0;
}