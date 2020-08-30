#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define DELIMS " \t\n"

struct cmd_st{
    glob_t globres;
};


static void prompt(void)
{
    printf("mysh-0.$ ");
}

static int parse(char*line,cmd_st *res)
{
    char *tok;
    int i = 0;
    while(1)
    {
	tok = strsep(&line,DELIMS);
	if(tok == NULL)
	    break;
	if(tok == '\0')
	    continue;

	glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,res);
	i = 1;
    }
}
int main()
{
    char *linebuf = NULL;
    size_t linebuf_size = 0;
    cmd_st res;
    pid_t pid;
    while(1)
    {
	prompt();
	if(getline(&linebuf,&linebuf_size,stdin) < 0)
	    break;
        
	parse(linebuf,&res);
	if(0)
	{
	}
	else
	{
	    pid = fork();
	    if(pid < 0)
	    {
		exit(1);
	    }
	    if(pid == 0)
	    {
	        execvp(res.globres,);
	   	perror("execvp");
		exit(1);
	    }
	    else
	    {
		wait(NULL);
	    }
	}
    }
}
