#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 3000000
#define RIGHT 3000200
int main()
{
    int mark = 0;
    pid_t pid;
    for(int i=LEFT;i < RIGHT; i++)
    {
	pid = fork();
	if(pid < 0)
	{
	    exit(1);
	}
	if(pid == 0)
	{
	    mark = 1;
	    for(int j=2;j<i/2;j++)
	    {
	   	 if(i % j == 0)
	    	{
			mark = 0;
			break;	
	    	}
	    }
	    if(mark)
	        printf("%d is a primerd.pid:%d\n",i,getpid());
	    exit(0);
	}
    }
}
