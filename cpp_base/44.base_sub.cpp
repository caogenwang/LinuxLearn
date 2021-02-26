#include <iostream>
using namespace std;

/*验证基类和子类在隐藏中的调用关系*/

class Base{
    public:
        void show1()
        {
            cout<<"Base show1"<<endl;
        }

        void show2()
        {
            cout<<"Base show2"<<endl;
        }

        virtual void show3(int a)
        {
            cout<<"Base show3"<<endl;
        }
};

class Sub:public Base{
    public:
        void show1(int a)
        {
            cout<<"Sub show1"<<endl;
        }

        void show2()
        {
            cout<<"Sub show2"<<endl;
        }

        virtual void show3()
        {
            cout<<"Sub show3"<<endl;
        }
};

int main()
{
    Base b;
    Sub s;

    b.show1();
    // s.show1();子类实现了父类当中同名的方法，但是参数不一样，此时父类的方法被隐藏，子类无法调用.必须显示指定
    s.Base::show1();
    s.show1(4);

    Base *pb = new Base();
    Sub *ps = new Sub();

    pb->show3(4);
    ps->Base::show3(2);//显示指定调用父类的show3
    ps->show3();

    b.show2();
    s.show2();
    s.Base::show2();

    return 0;
}