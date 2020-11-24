#include <iostream>
#include <string>
extern "C"{
    #include <stdio.h>
}
using namespace std;

class A{
public:
    int * ptr;
    virtual void f(){
        cout<<"A construct func"<<endl;
    }
    A(int *__ptr):ptr(__ptr){
        cout<<"A virtual func"<<endl;
        f();
    }
};

class B:public A{
public:
    B(int *__ptr):A(__ptr){
        cout<<"B construct func"<<endl;
        f();
    }
    virtual void f(){
        cout<<"B virtual func"<<endl;
    }
};

int main()
{
    int c = 0;
    int *ptr = &c;
    B b(ptr);
}
/*
item09：绝不在构造和析构过程中调用virtual函数：
这样调用带不来预想的结果
*/