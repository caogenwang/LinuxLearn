#include <stdio.h>
#include <stdlib.h>
#include <error.h>

int main(int argc,char**argv)
{
    FILE *fs = fopen("test.txt","rw");
    if (fs == NULL)
    {
        fclose(fs);
        perror("file open failed");
        exit(1);
    }
    char buf[1024] = {0};
    int n = 0;
    while ((n=fread(buf,1,5,fs))>0)
    {
        printf("%d\n",n);
    }
    fseek(fs,0,SEEK_END);
    long m = ftell(fs);
    printf("%d\n",m);
    fclose(fs);
    return 0;
}