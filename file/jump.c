#include <stdio.h>
#include <stdlib.h>

static int a()
{
    printf("%s():Begin.\n",__FUNCTION__);
    printf("call b().\n");
    b();
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