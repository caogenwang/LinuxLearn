#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int64_t mydu(const char *path)
{
    struct stat statres;
    lstat(path,&statres);
    
    if(!S_ISDIR(statres.st_mode))
    {
   	 return statres.st_blocks/2;
    }
    
    	
}
int main(int argc,char**argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

    
    return 0;
}
