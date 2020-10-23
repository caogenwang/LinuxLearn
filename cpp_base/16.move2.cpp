#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include <thread>

using namespace std;

//1. 移动不存在时，实行的是复制操作
class Foo
{
public:
    Foo(){}
    Foo(const Foo&)
    {
        cout <<"Foo(const Foo&)" << endl;
    }
};

//2. 移动速度未必比复制快
//2.1 辅助类（元素类）
class Widget
{
public:
    Widget() = default;
    Widget(const Widget&) {
        //模拟复制操作，假设需要1毫秒
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Widget(Widget&&) {
        //模拟移动操作，假设需要2毫秒
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    Widget& operator=(const Widget&) {
        //模拟复制赋值操作，假设需要1毫秒
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return *this;
    }
    Widget& operator=(Widget&&) {
        //模拟移动赋值操作，假设需要2毫秒
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        return *this;
    }
};

//2.2. 计算任意函数的执行时间：auto&&用于lambda表达式形参（C++14）
auto funcTimer = [](auto&& func, auto&& ... params)
{
    //计时器启动
    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();

    //调用func(param...)函数
    std::forward<decltype(func)>(func)(           //根据func的左右值特性来调用相应的重载&或&&版本的成员函数
        std::forward<decltype(params)>(params)... //保持参数的左/右值特性
        );

    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    cout << elapsed << " microseconds" << endl;
};
//2.3 复制和移动操作
auto lamMove = [](auto&& src) {
    auto dest = std::move(src);
    return;
};

auto lamCopy = [](auto&& src) {
    auto dest = src;
    return;
};

//2.4 测试vector类
void testVector()
{
    std::vector<Widget> vw1{ 10,Widget() };

    cout <<"copy vector: " ;
    funcTimer(lamCopy, vw1);

    //测试移动操作用时
    cout << "move vector: ";
    funcTimer(lamMove, vw1);
}

//2.5 测试array类
void testArray()
{
    std::array<Widget, 10> aw1;

    cout << "copy array: ";
    funcTimer(lamCopy, aw1);

    //测试移动操作用时
    cout << "move array: ";
    funcTimer(lamMove, aw1);
}

//3. move_if_noexcept的用法
struct Maythrow
{
    Maythrow() {}

    Maythrow(const Maythrow&) {
        cout <<"Maythrow copy construct." << endl;
    }

    Maythrow(Maythrow&&) {
        cout << "Maythrow move construct." << endl;
    }
};

struct Nothrow
{
    Nothrow() {}

    Nothrow(const Nothrow&) {
        cout << "Nothrow copy construct." << endl;
    }

    Nothrow(Nothrow&&) noexcept {  //注意，这里声明为noexcept!
        cout << "Nothrow move construct." << endl;
    }
};
int main()
{
    //1. 移动操作不存在时
    Foo f1;
    Foo f2 = std::move(f1); //调用复制构造函数

    //2. 移动速度未必比复制快
    testVector();
    testArray();

    //3. 移动未声明为noexcept时，调用复制构造
    Maythrow m;
    Nothrow  n;

    Maythrow mt = move_if_noexcept(m); //move_if_noexcept返回左值引用，调用复制构造函数
    Nothrow  nt = move_if_noexcept(n); //move_if_noexcept返回右值引用，调用移动构造函数

    return 0;
}
/*输出结果
Foo(const Foo&)
copy vector: 19825 microseconds
move vector: 5 microseconds     //常量时间
copy array: 19109 microseconds
move array: 29589 microseconds  //移动的速度未必比复制快！取决于Widget的移动和复制速度的比较！
Maythrow copy construct. //调用复制构造函数
Nothrow move construct.  //调用移动构造函数
*/