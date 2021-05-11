#include <iostream>
#include <unistd.h>
using namespace std;

class Base1   //基类
{
public:
    virtual void fun1()
    {
        cout << "Base1::fun1" << endl;
    }
    virtual void fun2()
    {
        cout << "Base1::fun2" << endl;
    }
private:
    int b1;
};
class Base2  //基类
{
public:
    virtual void fun1()
    {
        cout << "Base2::fun1" << endl;
    }
    virtual void fun2()
    {
        cout << "Base2::fun2" << endl;
    }
private:
    int b2;
};
class Derive : public Base1, public Base2  //派生类
{
public:
    virtual void fun1()
    {
        cout << "Derive::fun1" << endl;
    }
    virtual void fun3()
    {
        cout << "Derive::fun3" << endl;
    }
private:
    int d1;
};

typedef void(*FUNC) ();
void PrintVTable(int* VTable)
{
        cout << " 虚表地址: " << VTable << endl;
        for (int i = 0; VTable[i] != 0; ++i)    
        { 
           printf(" 第%d个虚函数地址 :0X%x,->", i, VTable[i]);         
           FUNC f = (FUNC)VTable[i];         
           f(); 
        }
        cout << endl;
}

int main()
{
    Derive d1;
    // int* VTable = (int*)(*(int*)&d1);
    // PrintVTable(VTable);
    // VTable = (int*)(*((int*)&d1 + sizeof (Base1)/4));
    // PrintVTable(VTable);
    d1.Base1::fun2();//需要显示指定那个父类
    d1.Base2::fun2();
    d1.fun1();
    d1.fun3();
}