#include "Widget.h"

#include<utility>
//将对string和vector和Gadget头文件的依赖从Wigdget.h转移动Wigdget.cpp文件中。如此，Widget类的使用者
//只需依赖Widget.h，而把复杂的依赖关系留给Widget的实现者(Widget.cpp)去处理
#include <string>
#include <vector>
using namespace std;

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

class Gadget {}; //本应#include "Gardget.h"，但为了简明起见，就直接在这里声明该类

//Widget::Impl的实现(包括此前在Widget中的数据成员)
struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget():pImpl(make_unique<Impl>())
{}

//注意：析构函数必须在Widget::Impl类之后定义。因为此时调用~Widget时，会调用unique_ptr的析构函数
//而unique_ptr中会调用delete删除其指向的对象，由于~Widget定义在Widget::Impl之后，因此这时看到的
//Impl是个完整的类，delete前通过了unique_ptr内部完整类型的判断！
/*
        这个地方由于在声明的时候，Impl是不完整的，在析构的时候，智能指针首先析构，然后调用Impl析构，此时若是Impl
        定义在~Widget()之前，则找不到Impl的完整定义，会报错；如果定义在之后，则不报错。这是由于智能指针首先析构调用
        指向对象析构的问题。

        析构函数调用顺序 ~Widget ->  unique_ptr  ->  Impl;
*/
Widget::~Widget() {}//或Widget::~Widget = default;

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

//make_unique(Ts&&... params)== std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
Widget::Widget(const Widget& rhs):pImpl(make_unique<Impl>(*rhs.pImpl))//深拷贝！
{
}

Widget& Widget::operator=(const Widget& rhs)
{
    *pImpl = *rhs.pImpl; //深拷贝！复制两个指针所指向的内容。pImpl本身是只移动类型
    return *this;
}