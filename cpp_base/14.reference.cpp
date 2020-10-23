#include <iostream>
#include <vector>

using namespace std;

class Widget {};

void func1(Widget&& param) {};  //param为右值引用类型(不涉及类型推导)

template<typename T>
void func2(T&& param){} //param为万能引用（涉及类型推导）


template<typename T>
void func3(std::vector<T>&& param) {} //param为右值引用，因为形式不是正好T&&
                                      //param的类型己确定为vector类型，而推导的是其元素的类型，
                                      //而不是param本身的类型。

template<typename T>
void func4(const T&& param){}  //param是个右值引用，因为被const修饰，其类型为const T&&，而不符”正好是T&&”的要求

template<class T>
class MyVector
{
public:
    void push_back(T&& x){} //x为右值引用。因为当定义一个MyVector对象后，T己确定。当调用该函数时T的类型不用再推导！
                            //如MyVector<Widget> v; v.push_back(...);时T己经是确定的Widget类型，无须再推导。
    template<class...Args>
    void emplace_back(Args&& ... args) {}; //args为万能引用，因为Args独立于T的类型，当调用该函数时，需推导Args的类型。
};


int main()
{
    //1. 模板函数形参（T&&）
    Widget w;
    func2(w); //func2(T&& param)，param为Widget&（左值引用）
    func2(std::move(w)); //param为Widget&&，是个右值引用。

    //2. auto&&
    int x = 0;
    Widget&& var1 = Widget();  //var1为右值引用（不涉及类型推导）
    auto&& var2 = var1;        //万能引用，auto&&被推导为Widget&&（右值引用）
    auto&& var3 = x;           //万能引用，被推导为int&;(左值引用）      

    //3. 计算任意函数的执行时间：auto&&用于lambda表达式形参（C++14）
    auto timefunc = [](auto && func, auto && ... params)
    {
        //计时器启动

        //调用func(param...)函数
        std::forward<decltype(func)>(func)(           //根据func的左右值特性来调用相应的重载&或&&版本的成员函数
            std::forward<decltype(params)>(params)... //保持参数的左/右值特性
            );
        
        //计时器停止并记录流逝的时间
    };

    timefunc(func1, std::move(w)); //计算func1函数的执行时间

    return 0;
}