
#include <iostream>
using namespace std;

class Base{
    public:
        void show1()
        {
            cout<<"Base show1"<<endl;
        }
        // virtual void show3(int a)
        // {
        //     cout<<"Base show3"<<endl;
        // }
};

class Sub:public Base{
    public:
        void show1()
        {
            cout<<"Sub show1"<<endl;
        }

        void show2()
        {
            cout<<"Sub show2"<<endl;
        }

        // virtual void show3()
        // {
        //     cout<<"Sub show3"<<endl;
        // }
};


int main()
{
    const int c_val = 233;
    int &use_val = const_cast<int&>(c_val);
    int *ptr_val = const_cast<int*>(&c_val);

    use_val = 666;
    cout<<c_val<<" "<<use_val<<" "<<*ptr_val<<endl;

    *ptr_val = 110;
    cout<<c_val<<" "<<use_val<<" "<<*ptr_val<<endl;

    /*基本类型的转换的时候和c语言风格一样*/
    double a = 10000000000.0;
    int b = static_cast<int>(a);
    cout<<b<<endl;


    /*层次类型转换，在基类转换成子类的时候没有安全检查；子类转基类是安全的*/
    Sub s;
    Base bb = static_cast<Base>(s);
    bb.show1();//Base show1

    Base *cc = new Base();
    Sub *ss = static_cast<Sub*>(cc);
    ss->show2();//Sub show2
    /*dynamic_cast可以进行类型安全检查*/

    cout<<"dynamic_cast"<<endl;
    Sub *sub = new Sub();
    sub->show1();//Sub show1
    Base *sub2base = dynamic_cast<Base*>(sub);
    if (sub2base != nullptr)
    {
        sub2base->show1();//Base show1
    }
    cout<<"address:"<<sub2base<<endl;//0x7fdea9402cd0

    cout<<"dynamic_cast base2sub"<<endl;
    /*测试子类到基类的转换*/
    Base *base = new Base();
    base->show1();
    Sub *base2sub = dynamic_cast<Sub*>(base);

    if (base2sub != nullptr)
    {
        base2sub->show1();//
    }
    cout<<"address:"<<base2sub<<endl;//0x0
    /*
        而从基类到子类的转换 , dynamic_cast 在转换时也没有报错,但是输出给 base2sub 
        是一个 nullptr ,说明dynami_cast 在程序运行时对类型转换对“运行期类型信息”
        （Runtime type information，RTTI）进行了检查
    */
    delete sub;

    return 0;
}
/*
233 666 666
233 110 110
*/