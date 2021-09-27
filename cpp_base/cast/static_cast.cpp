/*static_cast关键字的使用：
用法：static_cast <类型说明符> （变量或表达式）

它主要有如下几种用法：
    （1）用于类层次结构中基类和派生类之间指针或引用的转换
      进行上行转换（把派生类的指针或引用转换成基类表示）是安全的
      进行下行转换（把基类的指针或引用转换为派生类表示），由于没有动态类型检查，所以是不安全的
    （2）用于基本数据类型之间的转换，如把int转换成char。这种转换的安全也要开发人员来保证
    （3）把空指针转换成目标类型的空指针
    （4）把任何类型的表达式转换为void类型
    注意：static_cast不能转换掉expression的const、volitale或者__unaligned属性。

static_cast:可以实现C++中内置基本数据类型之间的相互转换。

如果涉及到类的话，static_cast只能在有相互联系的类型中进行相互转换,不一定包含虚函数。
*/

#include <iostream>

using namespace std;

class Base{
public:
    Base(){}
    ~Base(){}
     void display(){
        cout<<"Base"<<endl;
    }
int a{1};
int b{2};
};

class Derive:public Base{
public:
    Derive(){}
    ~Derive(){}
     void display(){
        cout<<"Derive"<<endl;
    }
    void func(){
        cout<<"func"<<endl;
    }
int c{3};
};


int main(){

    //（1）用于类层次结构中基类和派生类之间指针或引用的转换
    Base *ba = new Base();
    Derive *de = static_cast<Derive*>(ba);
    de->display();//改成虚函数就是base。普通函数是Derive
    cout<<de->a<<endl;//a = 1
    cout<<de->b<<endl;//b = 2
    cout<<de->c<<endl;//c = 0

    Derive *de1 = new Derive();
    ba = de1;
    ba->display();//虚函数是Derive
    cout<<de1->c<<endl;
    ba = static_cast<Base*>(de1);
    ba->display();//普通函数是Base，虚函数是Derive
    cout<<de->a<<endl;//a = 1
    cout<<de->b<<endl;//b = 2
    cout<<de->c<<endl;//c = 0//子类转换成父类，子类多余的数据被消掉了；父类转成父类，子类多出的数据没有赋值
    //对于类型的转换，父类到子类还是子类到父类，普通函数调用的都是转换类型的的函数；如果是虚函数，则调动原始类型的函数

    //（2）用于基本数据类型之间的转换，如把int转换成char。这种转换的安全也要开发人员来保证
        int a = 5;
        float b = static_cast<float>(a);
        cout<<b<<endl;

        double c = 10.002;
        int d = static_cast<int>(c);
        cout<<d<<endl;
    // 3）把空指针转换成目标类型的空指针
        int *ptrc = nullptr;
        void *ptrd1 = (void*)ptrc;
        void *ptrd2 = static_cast<void*>(ptrc);
    //（4）把任何类型的表达式转换为void类型
    


}
