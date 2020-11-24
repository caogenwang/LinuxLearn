#include <iostream>
#include <string>
extern "C"{
    #include <stdio.h>
}
using namespace std;

class A{
public:
    int * ptr;
    virtual void f(){}
    A(int *__ptr):ptr(__ptr){}
};

class B{
public:
    B(A a):pa(&a),ra(a){}
    A * pa;
    A & ra;
};

int main()
{
    int c = 0;
    int *p = &c;
    A a(p);
    a.ptr = p;
    B b(a);
    cout<< sizeof(a)<<endl;     //16
    cout<< sizeof(b)<<endl;     //16
    cout<< sizeof(b.ra)<<endl;  //16

    B *pb = &b;
    printf("%p\n",pb);
    printf("%p\n",pb+sizeof(b.pa));
    return 0;
}
/*
item09：绝不在构造和析构过程中调用virtual函数：
这样调用带不来预想的结果
*/