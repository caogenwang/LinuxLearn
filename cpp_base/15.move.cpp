#include <iostream>
#include <vector>
#include <utility>
using namespace std;

//1. 移动语义
class HugeMem
{
public:
    int* buff;
    int size;

    HugeMem(int size) : size(size > 0 ? size : 1)
    {
        buff = new int[size];
    }

    //移动构造函数
    HugeMem(HugeMem&& hm) noexcept : size(hm.size), buff(hm.buff)
    {
        hm.buff = nullptr;
    }

    ~HugeMem() { 
        delete[] buff;
    }
};

class Moveable
{
public:
    HugeMem h;
    int* i;
public:
    Moveable() : i(new int(3)), h(1024){}

    //移动构造函数（强制转为右值，以调用h的移动构造函数。注意m虽然是右值
    //引用，但形参是具名变量，m是个左值。因此m.h也是左值，需转为右值。
    Moveable(Moveable&& m) noexcept: i(m.i), h(std::move(m).h)
    {
        m.i = nullptr;
    }

    ~Moveable() { delete i; }
};

Moveable GetTemp()
{
    Moveable tmp = Moveable();

    cout << hex << "Huge mem from " << __func__
        << " @" << tmp.h.buff << endl;

    return tmp;
}

//2. 对常量对象实施移动将变成复制操作
class Annotation
{
    std::string value;
public:

    //注意：对常量的text对象实施移动操作时，由于std::move(text)返回的结果是个
    //const std::string对象，由于带const，不能匹配string(&& rhs)移动构造函数，
    //但匹配string(const string& rhs)复制构造函数，因此当执行value(std::move(text))
    //时，实际上是将text复制给value。对于非string类型的情况也一样，因此对常量对象的
    //移动操作实际上会变成复制操作！
    explicit Annotation(const std::string text) : value(std::move(text))
    {
    }
};

//3. 利用移动语义实现高性能的swap函数
template<typename T>
void Swap(T& a, T& b) noexcept  //声明为noexcept以便在交换失败时，终止程序
{
    //如果a、b是可移动的，则直接转移资源的所有权
    //如果是不可移动的，则通过复制来交换两个对象。
    T tmp(std::move(a)); //先把a的资源转交给tmp
    a = std::move(b);
    b = std::move(tmp);
}

int main()
{
    //1. 移动语义
    Moveable a(GetTemp()); //移动构造

    cout << hex << "Huge mem from " << __func__
        << " @" << a.h.buff << endl;

    return 0;
}
/*输出结果
Huge mem from GetTemp @02C66248 （从中可以看出Huge mem从临时对象移动了a对象）
Huge mem from main @02C66248
*/