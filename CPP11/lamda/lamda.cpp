#include <iostream>

using namespace std;

int gVal = 0;

//捕获this指针
class Test
{
private: 
    int i = 0;
public:
    void func(int x, int y)
    {
        int a = 0;
        //auto lamb1 = [] {return i; }; //error，无捕获列表。
        //auto lamb2 = [&i] {return i; }; //error, 不能捕获父作用域(func域）以外的变量(i)
        auto lamb3 = [=] {return i; }; //ok，按值捕获（含this指针），因此可以访问类中的成员变量(i)。
        auto lamb4 = [&] {return i + x + a; }; //ok，按引用捕获(含this指针），可以使用类中的成员变量(i)
                                               //同时，也捕获到形参x和局部变量a。
        auto lamb5 = [this] {return i; };  //ok，直接捕获this指针。

        auto lamb6 = [] {return gVal++; }; //ok，可以使用直接使用全局变量，无须也不能捕获它。
    }
};

int main()
{
    int a = 3;
    int b = 4;

    //1. lambda表达式初体验
    auto lamb1 = [] {};   //最简单的lambda表达式

    auto lamb2 = [=] {return a + b; };//省略参数列表和返回类型
    cout << lamb2() << endl; //7

    auto lamb3 = [&](int c) {b = a + c; };//省略返回类型，为void。
    //cout << lamb3(5) << endl; //error，返回void

    auto lamb4 = [] {return 1; }; //省略参数列表
    cout << lamb4() << endl;

    auto lamb5 = [=, &b](int c)->int {return b += a + c; }; //各部分完整的lambda表达式
    cout << "lamb5(2) = "<<lamb5(2) << ", b = " << b<< endl; //9, 9

    //2. lambda表达式的常量性及mutable关键字
    a = 1;
    //auto f1 = [] {return a++; };  //error，没有捕获外部变量
    //auto f2 = [=]() { a = 1;};    //error，const函数不能修改按值捕获的变量
    auto f2 = [=]() mutable { a = 2; }; //ok，被mutable修饰
    auto f3 = [&a]() { a = 3; };       //ok，按引用传递。const函数时影响引用本身，表示其不可修改
                                       //但其引用的内容不受const影响，仍可修改。
    //3. 捕获的时间点
    int x = 10;
    auto lambByVal = [x] {return x + 1; }; //按值捕获：声明时，x被复制一下
    auto lambByRef = [&x] {return x + 1; };//按引用捕获：x的值是随外部x的变化而变化。
    cout << "lambByVal() = "<< lambByVal() << endl; //11
    cout << "lambByRef() = "<< lambByRef() << endl; //11
    
    ++x;

    cout << "lambByVal() = " << lambByVal() << endl; //11
    cout << "lambByRef() = " << lambByRef() << endl; //12

    //4. lambda表达式转换为函数指针
    using FuncX = int(*)(int);
    using FuncXY = int(*)(int, int);

    int k = 1;
    auto lambN = [](int x, int y) {return x + y; };       //无捕获列表
    auto lambK = [&k](int x, int y) {return x + y + k; }; //有捕获列表

    FuncXY funcXY;
    funcXY = lambN; //ok，无捕获列表的lambda可转化为函数指针
    //lambN = funcXY; //error，不能将函数指针转为lambda
    //funcXY = lambK; //error，有捕获列表的lambda不能转为函数指针

    //5. 捕获this指针（见Test类）

    return 0;
}