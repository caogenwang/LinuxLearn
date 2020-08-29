#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 3000000
#define RIGHT 3000300
int main()
{
    int mark = 0;
    for(int i=LEFT;i < RIGHT; i++)
    {
	mark = 1;
	for(int j=2;j<i/2;j++)
	{
	    if(i % j == 0)
	    {
		mark = 0;
		break;	
	    }
	}
	if(mark)
	    printf("%d is a primer\n",i);	
    }

}
