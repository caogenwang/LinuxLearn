#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    pid_t pid;
    
    fflush(NULL);/*刷新缓冲区*/
    pid = fork();
    if(pid < 0)
    {
       perror("fork()");
       exit(1);
    }

    if(pid == 0)
    {
	printf("child process is working.%d\n",getpid());
    }
    else
    {  
	printf("parent process is working.%d\n",getpid());
    }
    getchar();
     exit(0);   
}
