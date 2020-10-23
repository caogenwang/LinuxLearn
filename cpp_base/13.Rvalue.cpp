#include <iostream>
#include <functional>
#include <utility>
#include <vector>
using namespace std;

class Widget
{
public:
    int x;
    int& rx = x;

    int arr[10];

    static int staticfunc(int x)
    {
        cout << "static int Widget::staticfunc(int x): " <<x << endl;
        return x;
    }

    int commonfunc(int x)
    {
        cout << "int Widget::commonfunc(int): " << x << endl;
        return x;
    }
};

Widget makeWidgetR()
{
    return Widget();
}

Widget& makeWidgetL()
{
    static Widget w; 
    return w;   //ok， Widget&是个引用类型，要注意不能返回局部对象。返回引用的时候不能是局部变量
}

Widget&& makeWidgetX()
{
    static Widget w;
    return std::move(w);  //ok。但要注意，Widget&&是个引用类型不能返回局部对象。右值引用，move(w)返回一个右值引用
}

//返回到函数的引用类型
using RetFunc = int(int);
int demoImpl(int i)
{
    cout << "int demo(int): " << i << endl;
    return i;
}

RetFunc&& RetFuncDemo()
{
    return demoImpl;
}


int main()
{
    //1. 常见的左/右值表达式分析

    //1.1 函数形参为左值
    //int test(int&& x){return x;} //形参x为左值（具名变量），尽管其为右值引用类型。

    int i = 0;
    //1.2 前置/后置自增、自减表达式
    int&& ri = i++;  //i++为右值，表达式返回的是i的拷贝，匿名对象是个右值，表达式一般都是右值
    int& li = ++i;   //++i返回i本身，是个具名对象，为左值。

    int& r2 = ri;    //虽然ri的类型是int&&，但ri本身是个具名变量。因此仍为左值。
    //int&& r3 = ri; //error,ri是个左值
    r2 = 5;
    cout << "ri = " << ri << ", i = "<< i << endl;  //5, 2，ri是一个左值，r2是ri的引用，修改r2，也就是修改ri

    //1.3 解引用和取地址运算表达式
    int* p = &i;
    int& lp = *p;   //解引用：*p为左值，因为可以对*p取址址&(*p)。或*p = 5;
    int*&& rp = &i; //取地址：&i是个内存地址，是个右值。可以用来初始化右值引用
    *rp = 10;
    cout  <<"i = "<< i << endl; //i = 10，i的值通过rp引用修改。rp还是一个左值，存放着i的地址

    //1.4 字面量
    const char(&hw)[13] = "hello world!"; //字符串字面量是左值，可以用于初始化左值引用
    cout << "const char(&hw)[13] = " << hw << endl;

    int&& ten = 10;  //10为纯右值

    //1.5 赋值表达式 和 算术表达式、比较表达式、逻辑表达式
    int& a = (i += 2);  //i +=2为赋值表达式，结果为左值。类似的，还有 a = b、a %= b
    int&& b = i + 2;    //i+2为算术表达式，结果为右值。类似的还有a + b、a % b、a & b、a << b
    int&& c = (a > b);  //比较表达式结果为右值。类似的还有：a < b、a == b、a >= b
    int&& d = (a && b); //逻辑表达式结果为右值。类似的还有：a && b、a || b、!a

    //1.6 lambda表达式
    //auto& lam = [](int x, int y) {return x + y; };//lambda表达式为纯右值，不能绑定到左值

    //2.下标表达式
    int arr[10]; 
    int& ra1 = arr[2]; //[]下标表达式返回左值引用，仍是左值
    vector<int> vec{ 1,2,3,4 };
    int& rv = vec[2];  //operator[]返回左值引用，是个左值表达式。

    //3.对象访问表达式
    Widget w1;
    int& rx1 = w1.x;         //w1为左值，所以w1.x为左值
    int&& rx2 = Widget().x;  //Widget()是个临时对象（右值）。因此，Widget().x为右值。
    //int& rx3 = Widget().x; //error，理由同上。
    
    using WidgetStaticFunc = int(int);
    WidgetStaticFunc& wsf = w1.staticfunc; //静态成员函数，是个左值。
    wsf(10);
    w1.commonfunc(2);   //w1.commomfunc是个纯右值，只能用于函数调用，不能做其它用途。
    
    //4. 类型转换表达式
    int&& i1 = std::move(i);              //std::move()返回值为右值引用类型，是个右值。
    int&& i2 = static_cast<int&&>(i);     //转换为右值引用类型，表达式结果是个右值
    double&& d1 = static_cast<double>(i); //转换为右值类型，结果是个右值
    int& i3 = static_cast<int&>(i);       //转换为左值引用类型，表达式结果为左值

    //5.函数返回型类型
    Widget&  w2 = makeWidgetL();   //返回左值引用类型，为左值表达式
    Widget&& w3 = makeWidgetR();   //返回非引用类型，为右值表达式
    Widget&& w4 = makeWidgetX();   //返回右值引用类型，为右值表达式
    RetFunc& rf1 = RetFuncDemo();  //RetFuncDemo返回一个到函数的右值引用，是个左值表达式(C++11的标准行为）。
    RetFunc&& rf2 = RetFuncDemo(); //RetFuncDemo仍可以用来初始化右值引用！
    rf1(5);

    return 0;
}
/*输出结果
ri = 5, i = 2
i = 10
const char(&hw)[13] = hello world!
static int Widget::staticfunc(int x): 10
int Widget::commonfunc(int): 2
int demo(int): 5
*/