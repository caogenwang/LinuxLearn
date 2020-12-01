#include <iostream>
#include <numeric>
#include <functional>

using namespace std;

int myfunc(int x,int y){return x+2*y;}

struct myclass
{
    int operator()(int x,int y){return x+2*y;}
}myobject;

void test_accumulate()
{
    int init = 100;
    int nums[] = {10,20,30};

    cout<<"using default acumulate: ";
    cout<< accumulate(nums,nums+3,init);//160，init是100，一直加上nums的元素
    cout<<"\n";

    cout<<"using functiona minux<int>: ";
    cout<< accumulate(nums,nums+3,init,minus<int>());//40，init是100，一直减去nums的元素
    cout<<"\n";

    cout<<"using custom function: ";
    cout<< accumulate(nums,nums+3,init,myfunc);//220,init 是x，nums的元素是y
    cout<<"\n";

    cout<<"using custom obj: ";
    cout<< accumulate(nums,nums+3,init,myobject);//220,init 是x，nums的元素是y
    cout<<"\n";
}

int main()
{
    test_accumulate();
    return 0;
}