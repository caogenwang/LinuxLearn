#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{	
	int pd[2];
	char buf[1024];
	if(pipe(pd)<0)
	{
		perror("pipe()");
		exit(1);
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		
	}
	if(pid == 0)
	{
		close(pd[1]);
		int len = read(pd[0],buf,1024);
		write(1,buf,len);
		close(pd[0]);
		exit(0);
	}
	else
	{
		close(pd[0]);
		write(pd[1],"hello\n",6);
		close(pd[1]);
		wait(NULL);
		exit(0);		
	}
	exit(0);
}
