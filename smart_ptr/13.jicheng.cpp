#include <iostream>

using namespace std;

class A{
    public:
    virtual void display()
    {
        cout<<"I am A"<<endl;
    }
};

class B:public A{
    public:
    void display()
    {
        cout<<"I am B"<<endl;
    }
};

void func(A *a)
{
    a->display();
}

void func2(B*b)
{
    b->display();
}

int main()
{
    A *a = new B();
    B *b = new B();
    A *a1 = new A();
    // a->display();
    // a->A::display();//强制访问A的display函数
    func(b);
    func(a1);//形参是基类时，可以传入子类的对象；反之则不可以。得出结论，派生类可以向基类转换
    // func(b);Error
    return 0;
}