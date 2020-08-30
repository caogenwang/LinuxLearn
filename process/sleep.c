#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{

    printf("Begin:%s\n",__FUNCTION__);
    execl("/bin/sleep","sleep","100",NULL);
    perror("execl");
    exit(1);
    printf("End\n");
}
