#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
static jmp_buf save;
static int b();
static int c();
static int a()
{
    int ret;
    printf("%s():Begin.\n",__FUNCTION__);
   
    ret = setjmp(save);
    if (ret == 0)
    {
         printf("call b().\n");
         b();
         printf("%s():b()return.\n",__FUNCTION__);
    }
    else
    {
        printf("%s():jump back here.%d\n",__FUNCTION__,ret);
    }
    
    
    printf("%s():End.\n",__FUNCTION__);
    return 0;
}
static int b()
{
    printf("%s():Begin.\n",__FUNCTION__);
    printf("call c().\n");
    c();
    printf("%s():End.\n",__FUNCTION__);
    return 0;
}
static int c()
{
    printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():jump now.\n",__FUNCTION__);
    longjmp(save,6);
    printf("%s():End.\n",__FUNCTION__);
    return 0;
}
int main()
{
    printf("%s():Begin.\n",__FUNCTION__);
    printf("call a().\n");
    a();
    printf("%s():End.\n",__FUNCTION__);
    return 0;
}
