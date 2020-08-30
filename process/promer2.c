#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LEFT 3000000
#define RIGHT 3000400
#define N 3
int main()
{
    int mark = 0;
    pid_t pid;
	for (int n = 0; n < N; n++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if (pid == 0)
		{
			for(int i=LEFT;i < RIGHT; i+=N)
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
				printf("[%d]%d is a primerd.pid:%d\n",n,i,getpid());
			}
			exit(0);
		}
		
	}

	for (int i = 0; i < N; i++)
	{
		printf("wait....\n");
		wait(NULL);
	}
	return 0;
}
