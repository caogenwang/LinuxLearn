#include <iostream>
#include <stdlib.h>
using namespace std;
typedef struct Test_s
{
    int a;
    int b;
}Test_t;

int main()
{

    cout<<"int pointer test:"<<endl;
    int *pi = (int*)malloc(sizeof(int)*10);
    for(int i=0;i<10;i++)
    {
        cout<<"pi:"<<pi<<endl;
        pi++;//4字节增加
    }
    cout<<"char pointer test:"<<endl;
    char *pc = (char*)malloc(10);
    if (NULL != pc)
    {
        for(int i=0;i<10;i++)
        {
            cout<<"pc:"<<pc<<endl;
            pc++;
        }
    }
    Test_t *t = (Test_t*)malloc(sizeof(Test_t)*10);
    if (NULL != t)
    {
        for(int i=0;i<10;i++)
        {
            cout<<"t:"<<t<<endl;
            t++;
        }
    }
    
    return 0;
}