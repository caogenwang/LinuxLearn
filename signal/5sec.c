#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    time_t end;
    int count = 0;
    
    end = time(NULL) + 5;
    while(time(NULL) <= end)
        count++;
    printf("%lld\n",count);
    exit(0);
}
