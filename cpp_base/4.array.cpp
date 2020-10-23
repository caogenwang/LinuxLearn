#include <iostream>
using namespace std;


int (*f)(int,int);  
int func(int a,int b)
{
    cout<<a<<endl;
    cout<<b<<endl;
    return 0;
}

int main()
{
    // int a[4][4] ={
    //     {0,1,2,3},
    //     {4,5,6,7},
    //     {8,9,10,11},
    //     {12,13,14,15}
    // };
    // cout<<a<<endl;
    // cout<<a+1<<endl;
    // cout<<a+2<<endl;
    // cout<<a+3<<endl;

    
    // cout<<a[0]+2<<endl;

    f = func;
    f(1,2);
}

/*
数据类型 (*函数指针名)(形参列表);  
int (*f)(int,int); 
int &ref = 5;                //错误
const int &ref = 5;                //正确

int a[10];
int &ref=a;                //错误
int &ref[10]=a;                //错误
int &ref=a[0];                //正确
int &*ref=a;                //错误
*/

/*
float &ref = * new float;            //需要注意：*
ref = 200;
cout << ref;
delete &ref;                //需要注意：&
*/