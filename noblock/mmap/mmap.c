#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char*argv[])
{

	int fd = open("test.txt",O_RDONLY);
	if(fd < 0)
	{
		perror("open()");
		exit(1);
	}
	struct  stat statres;
	int n = fstat(fd,&statres);
	if(n < 0)
	{
		perror("fstat()");
		exit(1);
	}
	printf("size:%d\n",statres.st_size);
	char *p = mmap(NULL,statres.st_size,PROT_READ,MAP_SHARED,fd,0);
	if(p == MAP_FAILED)
	{
		perror("mmap()");
		exit(1);		
	}
	close(fd);
	int i = 0;
	int count = 0;
	for(i;i<statres.st_size;i++)
	{
		if(p[i] == 'a')
			count++;		
	}
	printf("%d\n",count);	
	exit(0);
}
