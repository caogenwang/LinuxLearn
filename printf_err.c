#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
int main()
{

    // write(2, "err\n", 10);
    int fd = open("/Users/yangsen/work/LinuxLearn/README.md",O_RDWR);
    void *p=mmap(NULL,6,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p==MAP_FAILED){
		perror("mmap");
		return -1;
	}
    return 0;
}