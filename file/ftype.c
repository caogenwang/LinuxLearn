#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int ftype(const char*fname)
{
    struct stat statres;
    if (stat(fname,&statres) < 0)
    {
        perror("stat()");
        exit(1);    
    }   
    if(S_ISREG(statres.))   
    return statres.st_size;
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

   int n =  flen(argv[1]);
   printf("%d\n",n);
    return 0;
}
