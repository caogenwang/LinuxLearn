#include <stdio.h>
#include <stdlib.h>

int main(int argc,char**argv)
{
    int year=2014;
    int month = 5;
    int day = 30;
    char buf[1024]={0};

    sprintf(buf,"%d-%d-%d",year,month,day);
    return 0;
}