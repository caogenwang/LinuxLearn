#include<iostream>

using namespace std;

class base
{
public :
    void m(){cout<<"m"<<endl;}
};
 
class derived : public base
{
public:
    void f(){cout<<"f"<<endl;}
};
 
int main()
{
    derived * p;
    // p = new base;//error: assigning to 'derived *' from incompatible type 'base *'
    p = dynamic_cast<derived *>(new base);
    // 在本例中利用dynamic_cast进行强制类型转换，但是因为base类中并不存在虚函数，
    //因此p = dynamic_cast<derived *>(new base);这一句会编译错误。
    // 为了解决本例中的语法错误，我们可以将base类中的函数m声明为虚函数，virtual void m(){cout<<"m"<<endl;}。
    // dynamic_cast还要求<>内部所描述的目标类型必须为指针或引用
    p->m();
    p->f();
    return 0;
}