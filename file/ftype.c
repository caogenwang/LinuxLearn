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
    if(S_ISREG(statres.st_mode))   
        return '-';
    else if(S_ISDIR(statres.st_mode))
        return 'd';
    else if(S_ISBLK(statres.st_mode))
        return 'b';
    else
        return 'x';
        
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

   int n =  ftype(argv[1]);
   printf("%c\n",n);
    return 0;
}
