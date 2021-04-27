#include<iostream>
using namespace std;

int main()
{

    auto func = [](int a,int b)->int{return a+b;};

    []{};
    int a = 10;
    int b = 20;
    [=]{return a + b;};

    auto fun1 = [&](int c){b = a + c; };
    auto fun2 = [=,&b](int c)->int{return b+=c+a;};
    return 0;
}