#include <iostream>
#include <unistd.h>
using namespace std;
class AA
{
public:
    virtual void func1()
    {
        cout << "AA::func1" << endl;
    }

public:
    int _a;
};
class BB:virtual public AA
{
public:
    virtual void func1()
    {
        cout << "BB::func1" << endl;
    }

public:
    int _b;
};
class CC :virtual public AA
{
public:
    virtual void func1()
    {
        cout << "CC::func1" << endl;
    }

public:
    int _c;
};
class DD :public BB, public CC
{
public:
    virtual void func1()
    {
        cout << "DD::func1" << endl;
    }
    virtual void func2()
    {
        cout << "DD::func2" << endl;
    }
public:
    int _d;
};
typedef void(*FUNC) ();
void PrintVTable(int* VTable)
{
        cout << " 虚表地址: " << VTable << endl;
        // for (int i = 0; VTable[i] != 0; ++i)    
        // { 
        //    printf(" 第%d个虚函数地址 :0X%x,->", i, VTable[i]);         
        //    FUNC f = (FUNC)VTable[i];         
        //    f(); 
        // }
        cout << endl;
}
int main()
{
    DD d1;
    d1.BB::_a = 1;
    d1.CC::_a = 2;
    d1._b = 3;
    d1._c = 4;
    d1._d = 5;
    cout << sizeof(d1) << endl;//打印d1的字节长度
    int* Vtable1 = (int*)(*(int*)&d1);
    PrintVTable(Vtable1);
    pause();
    return 0;
}