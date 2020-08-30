#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{

    printf("Begin:%s\n",__FUNCTION__);
    execl("/bin/ls","ls","-l",NULL);
    perror("execl");
    exit(1);
    printf("End\n");
}
