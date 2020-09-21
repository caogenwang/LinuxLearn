#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define TTY1 "dev/tty11"
#define TTY2 "dev/tty12"
#define TTY3 "dev/tty34"
#define TTY4 "dev/tty43"

#define BUFSIZE 1024
int main()
{
	int fd1,fd2,fd3,fd4;
	fd1 = open(TTY1,O_RDWR);
	if(fd1 < 0)
	{
		perror("open()1");
		exit(1);
	}
	write(fd1,"TTY1\n",5);
	fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
	if(fd2 < 0)
	{
		perror("open()2");
		exit(1);
	}
	write(fd2,"TTY2\n",5);
	rel_addjob(fd1,fd2);

	fd3 = open(TTY3,O_RDWR);
        if(fd3 < 0)
        {       
              perror("open()3");
              exit(1);
        }
          write(fd1,"TTY3\n",5);
          fd4 = open(TTY4,O_RDWR|O_NONBLOCK);
          if(fd4 < 0)
          {       
                  perror("open()4");
                  exit(1);
          }
          write(fd4,"TTY4\n",5);
          rel_addjob(fd3,fd4);

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
}

