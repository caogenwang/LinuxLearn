#include <stdio.h>
#include <stdlib.h>

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

    FILE *fp;
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    char *linebuf=NULL;
    size_t linesize=0;
    while (getline(&linebuf,&linesize,fp) > 0)
    {
        printf("line:%s\n",linebuf);
    }
    fclose(fp);  
}