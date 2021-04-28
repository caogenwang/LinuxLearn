// 一. unique_ptr的基本用法

// （一）初始化方式

// 　　1. 直接初始化：unique<T> myPtr(new T);  //ok。但不能通过隐式转换来构造，如unique<T> myPtr = new T()。因为unique_ptr构造函数被声明为explicit。

// 　　2. 移动构造：unique<T> myOtherPtr = std::move(myPtr);但不允许复制构造，如unique<T> myOther = myPtr; 因为unique是个只移动类型。

// 　　3. 通过make_unique构造：unique<T> myPtr = std::make_unique<T>(); //C++14支持的语法。但是make_都不支持添加删除器，或者初始化列表。

// 　　4. 通过reset重置：如std::unique_ptr up; up.reset(new T());

// （二）指定删除器

// 　　1. unique_ptr<T,D>  u1(p,d);删除器是unique_ptr类型的组成部分，可是普通函数指针或lambda表达式。注意，当指定删除器时需要同时指定其类型，即D不可省略。

// 　　2.使用默认的deleter时，unique_ptr对象和原始指针的大小是一样的。当自定义deleter时，如果deleter是函数指针，则unique_ptr对象的大小为8字节。对于函数对象的deleter，unique_ptr对象的大小依赖于存储状态的多少，无状态的函数对象（如不捕获变量的lambda表达式），其大小为4字节。

// 二. 剖析unique_ptr

// （一）源码分析【节选】

//指向单对象
template <class _Ty, class _Dx> //注意，删除器也是unique_ptr类型的一部分
class unique_ptr { // non-copyable pointer to an object
private:
    _Compressed_pair<_Dx, pointer> _Mypair;
public:

    using pointer      = _Ty*;//裸指针类型
    using element_type = _Ty; //对象类型
    using deleter_type = _Dx; //删除器类型

    template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr() noexcept : _Mypair(_Zero_then_variadic_args_t()) {} //构造一个空的智能指针

    unique_ptr& operator=(nullptr_t) noexcept; //重置指针为nullptr

    //注意，explicit阻止隐式构造,如unique_ptr<int> up = new int(100);编译错误。只能显示构造，如unique_ptr<int> up(new int(100));
    template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
    explicit unique_ptr(pointer _Ptr) noexcept : _Mypair(_Zero_then_variadic_args_t(), _Ptr) {} 

    template <class _Dx2 = _Dx, enable_if_t<is_constructible_v<_Dx2, const _Dx2&>, int> = 0>
    unique_ptr(pointer _Ptr, const _Dx& _Dt) noexcept : _Mypair(_One_then_variadic_args_t(), _Dt, _Ptr) {}

    unique_ptr(unique_ptr&& _Right) noexcept;  //移动构造

    unique_ptr& operator=(unique_ptr&& _Right) noexcept;//移动赋值

    void swap(unique_ptr& _Right) noexcept;//交换两个智能指针所指向的对象

    ~unique_ptr() noexcept; //析构函数，调用删除器释放资源。

    Dx& get_deleter() noexcept; //返回删除器

    const _Dx& get_deleter() const noexcept;//返回删除器

    add_lvalue_reference_t<_Ty> operator*() const; //解引用

    pointer operator->() const noexcept; //智能指针->运算符

    pointer get() const noexcept; 

    explicit operator bool() const noexcept; //类型转换函数，用于条件语句，如if(uniptr)之类

    pointer release() noexcept; //返回裸指针，并释放所有权

    void reset(pointer _Ptr = pointer()) noexcept ; //重置指针,这个地方会析构原始指针

    unique_ptr(const unique_ptr&) = delete; //不可拷贝
    unique_ptr& operator=(const unique_ptr&) = delete; //不可拷贝赋值
};

//指向数组类型
template <class _Ty, class _Dx>
class unique_ptr<_Ty[], _Dx> { 
private:
    _Compressed_pair<_Dx, pointer> _Mypair; 
public:
    using pointer      = typename _Get_deleter_pointer_type<_Ty, remove_reference_t<_Dx>>::type;
    using element_type = _Ty;
    using deleter_type = _Dx;

    //...    //省略了与unique_ptr单对象类型相同的一些操作
   
    ~unique_ptr() noexcept; //析构函数，调用删除器释放资源。

    _Ty& operator[](size_t _Idx) const {  //数组[]操作符
        return _Mypair._Myval2[_Idx];
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
};
/*
1. unique_ptr的构造函数被声明为explicit，禁止隐式类型转换的行为。原因如下：

　　  ①可减少误将智能指针指向栈对象的情况。如unique_ptr<int> ui = &i;其中的i为栈变量。

　　  ②可避免将一个普通指针传递给形参为智能指针的函数。假设，如果允许将裸指针传给void foo(std::unique_ptr<T>)函数，则在函数结束后会因形参超出作用域，裸指针将被delete的误操作。

　　2. unique_ptr的拷贝构造和拷贝赋值均被声明为delete。因此无法实施拷贝和赋值操作，但可以移动构造和移动赋值。

　　3. 删除器是unique_ptr类型的一部分。默认为std::default_delete，内部是通过调用delete来实现。

　　4. unique_ptr可以指向数组，并重载了operator []运算符。如unique_ptr<int[]> ptr(new int[10]); ptr[9]=9;但建议使用使作std::array、std::vector或std::string来代替这种原始数组。

（二）常用操作

　　1.get()：返回unique_ptr中保存的裸指针

　　2.reset()：重置unique_ptr。

　　3.release()：放弃对指针的控制权，返回裸指针，并将unique_ptr自身置空。通常用来初始化另一个智能指针。

　　4.swap(q)：交换两个智能指针所指向的对象。

【编程实验】std::unique_ptr的基本用法

*/

#include <iostream>
#include <vector>
#include <memory>  //for smart pointer

using namespace std;

class Widget {};

//返回值RVO优化：
unique_ptr<int> func()
{
    unique_ptr<int> up(new int(100));
    return  up; //up是个左值，调用拷贝构造给返回值？ No。
                //C++标准要求当RVO被允许时，要么消除拷贝，要么隐式地把std::move用在要返回的局部
                //对象上去。这里编译器会直接在返回值位置创建up对象。因此根本不会发生拷贝构造，
                //unique_ptr本身也不能被拷贝构造。

    //return unique_ptr<int>(new int(100)); //右值，被移动构造。
}

void foo(std::unique_ptr<int> ptr)
{
}

void myDeleter(int* p)
{
    cout << "invoke deleter(void* p)"<< endl;
    delete p;
}

int main()
{
    //1. unique_ptr的初始化
    //1.1 通过裸指针创建unique_ptr（由于unique_ptr的构造函数是explicit的，必须使用直接初始化，不能做隐式类型转换）
    std::unique_ptr<Widget> ptr1(new Widget);      //ok; 直接初始化
    //std::unique_ptr<Widget> ptr1 = new Widget(); //error。不能隐式将Widget*转换为unqiue_ptr<Widget>类型。

    std::unique_ptr<int[]> ptr2(new int[10]); //指向数组

    //1.2 通过移动构造
    //std::unique_ptr<Widget> ptr3 = ptr1;    //error，unique_ptr是独占型，不能复制构造
    std::unique_ptr<Widget> ptr3 = std::move(ptr1);  //ok，unique_ptr是个只移动类型，可以移动构造
    auto ptr4 = std::move(ptr3);     //ok， ptr4为unique_ptr<Widget>类型

    //1.3 通过std::make_unique来创建
    auto ptr5 = std::make_unique<int>(10);

    //auto ptr6 = std::make_unique<vector<int>>({1,2,3,4,5}); //error，make_unique不支持初始化列表
    auto initList = { 1,2,3,4,5 };
    auto ptr6 = std::make_unique<vector<int>>(initList);

    //2. 传参和返回值
    int* px = new int(0);
    //foo(px); //error，px无法隐式转为unique_ptr。可防止foo函数执行完毕后，px会自动释放。
    //foo(ptr5); //error，智能指针不能被拷贝。因此，可以将foo的形参声明为引用，以避免所有权转移
    foo(std::move(ptr5)); //ok，通过移动构造

    auto ptr7 = func(); //移动构造

    //3.常用操作
    std::unique_ptr<Widget> upw1; //空的unique_ptr
    upw1.reset(new Widget);
    std::unique_ptr<Widget> upw2(new Widget);

    cout <<"before swap..." << endl;
    cout << "upw1.get() = " << hex << upw1.get() << endl;

    cout << "upw2.get() = " << hex << upw2.get() << endl;

    cout << "after swap..." << endl;
    upw1.swap(upw2); //交换指针所指的对象
    cout << "upw1.get() = " << hex << upw1.get() << endl;
    cout << "upw2.get() = " << hex << upw2.get() << endl;

    //upw1.release(); //release放弃了控制权不会释放内存，丢失了指针
    Widget* pw = upw1.release();//放弃对指针的控制
    delete pw; //需手动删除

    if (upw1) {  //unique_ptr重载了operator bool()
        cout << "upw1 owns resourse" << endl;
    }else {
        cout << "upw1 lost resourse" << endl;
    }

    upw1.reset(upw2.release()); //转移所有权
    cout << "upw1.get() = " << hex << upw1.get() << endl;
    cout << "upw2.get() = " << hex << upw2.get() << endl;

    //upw1 = nullptr; //释放upw1指向的对象，并将upw1置空
    //upw1.reset(nullptr);

    //4.unique_ptr的大小
    std::unique_ptr<int,decltype(&myDeleter)> upd1(new int(0), myDeleter); //自定义删除器
    auto del = [](auto* p) {delete p; };
    std::unique_ptr<int, decltype(del)> upd2(new int(0), del); 
    cout << sizeof(upw1) << endl; //4字节，默认删除器
    cout << sizeof(upd1) << endl; //8字节
    cout << sizeof(upd2) << endl; //4字节

    return 0;
}
// 三. 使用场景

// （一）作为工厂函数的返回类型

// 　　1. 工厂函数负责在堆上创建对象，但是调用工厂函数的用户才会真正去使用这个对象，并且要负责这个对象生命周期的管理。所以使用unique_ptr是最好的选择。

// 　　2. unique_ptr转为shared_ptr很容易，作为工厂函数本身并不知道用户希望所创建的对象的所有权是专有的还是共享的，返回unique_ptr时调用者可以按照需要做变换。

// （二）PImpl机制：（Pointer to Implemention）

// 　　1. 操作方法

// 　　（1）将曾经放在主类中的数据成员放到实现类中去，然后通过指针间接地访问那些数据成员。此时主类中存在只有声明而没有定义的类型（也叫非完整类型），如Widget::Impl。

// 　　（2）在实现类中，动态分配和归还原那些原本应在主类中定义的那数据成员对象。即将这个数据成员放到实现类中定义（动态分配其内存）

// 　　2. 注意事项

// 　　（1）PImpl机制通过降低类的客户和类实现者之间的依赖性，减少了构建遍数。

// 　　（2）对于采用std::unique_ptr来实现的PImpl指针，须在类的头文件中声明特殊成员函数，但在实现文件中实现它们（注意，不能直接在头文件中实现，具体原因见《编程实验》中的说明）。如，必须同时声明并实现类的析构函数。再由于自定义了析构函数，编译器不再提供默认的移动构造和移动赋值函数，如果需要这些函数，则也必须在头文件中声明，并在实现类中去实现。

// 　　（3）上述建议仅适用于std::unique_ptr，但并不适用于std::shared_ptr。因为删除器在unique_ptr中是其类型的一部分，而在shared_ptr中则不是。声明对象时，unique_ptr<T>支持T是个非完整类型，但在析构时T必须己经是个完整的类型。unique_ptr析构时会先判断T是否为完整类型再调用delete删除其所指对象，但shared_ptr<T>则不会。

// 【编程实验】unique_ptr的使用场合

#ifndef  _WIDGET_H_
#define _WIDGET_H_
#include <memory>

//1.传统的做法
//问题：数据成员会导致Widget.h文件必须include <string>
//      <vector>和gadget.h。当客户包含Widget.h里，会增加编译时间，而且
//      如果其中的某个头文件(如Gadget.h)发生改变，则Widget的客户必须重新编译！
//class Widget
//{
//    std::string name;
//    std::vector<double> data;
//    Gadget g1, g2, g3;// //自定义类型，位于gadget.h。
//public:
//    Widget();
//};

//2. 采用PImpl手法
class Widget
{
    //声明实现结构体以及指向它的指针
    struct Impl; //注意只有声明，没实现。是个非完整类型。
    std::unique_ptr<Impl> pImpl; //使用智能指针而非裸指针。这里声明一个指针非完整类型的指针。注意针对非完整
                                 //类型，可以做的事情极其有限。由于unique_ptr中会将删除器作为其类型的一部分
                                 //因此，但unique_ptr析构被调用时，当delete其所指对象时，会先判断T是否是个完
                                 //整类型。如果不是，则会报错。因此必须在pImpl被析构前，确保Impl被定义（即是个完整类型）
                                 //因此，使用unique_ptr<非完整类型时>，必须为该类同时定义析构函数！具体原因见后面的分析。

    //std::shared_ptr<Impl> pImpl; //由于删除器不是shared_ptr类型的组成部分。当pImpl被析构时，不会判断T是否为完整类型。
                                   //因此，不要求Widget必须自定义析构函数。

public:
    Widget();
    ~Widget(); //Impl是个非完整类型，这里必须声明析构函数，并在Widget.cpp中实现它。
                //注意，不能在该文件中实现，因为此时unique_ptr看到的Impl是个非完整类型，unique_ptr内部要求delete前，其
                //其指向的必须是个完整类的指针。

    //移动构造和移动赋值（由于自定义了析构函数，所以编译器不再提供默认的移动构造和移动赋值函数，这里需手动填加）
    Widget(Widget&& rhs); //只能声明，须放在.cpp中去实现。编译器会在move构造函数内抛出异常的事件中生成析构pImpl代码，
                          //而此处Impl为非完整类型。
    Widget& operator=(Widget&& rhs); //只能声明，须放在.cpp中去实现。因为移动赋值pImpl时，需要先析构pImpl所指对象，但
                                     //此时仍为非完整类型。

    //让Widget支持复制操作。注意unique_ptr不可复制
    Widget(const Widget& rhs);  //仅声明
    Widget& operator=(const Widget& rhs); //仅声明
};

#endif // ! _WIDGET_H_

#include "Widget.h"

//将对string和vector和Gadget头文件的依赖从Wigdget.h转移动Wigdget.cpp文件中。如此，Widget类的使用者
//只需依赖Widget.h，而把复杂的依赖关系留给Widget的实现者(Widget.cpp)去处理
#include <string>
#include <vector>
class Gadget {}; //本应#include "Gardget.h"，但为了简明起见，就直接在这里声明该类

//Widget::Impl的实现(包括此前在Widget中的数据成员)
struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget():pImpl(std::make_unique<Impl>())
{}

//注意：析构函数必须在Widget::Impl类之后定义。因为此时调用~Widget时，会调用unique_ptr的析构函数
//而unique_ptr中会调用delete删除其指向的对象，由于~Widget定义在Widget::Impl之后，因此这时看到的
//Impl是个完整的类，delete前通过了unique_ptr内部完整类型的判断！
Widget::~Widget() {}//或Widget::~Widget = default;

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

//make_unique(Ts&&... params)== std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
Widget::Widget(const Widget& rhs):pImpl(std::make_unique<Impl>(*rhs.pImpl))//深拷贝！
{
}

Widget& Widget::operator=(const Widget& rhs)
{
    *pImpl = *rhs.pImpl; //深拷贝！复制两个指针所指向的内容。pImpl本身是只移动类型
    return *this;
}

#include <iostream>
#include <memory>
#include <functional>
#include "Widget.h"
using namespace std;

enum class InvestmentType {itSock, itBond, itRealEstate};
class Investment//投资
{
public:
    virtual ~Investment() {} //声明为virtual,以便正确释放子类对象
};

class Stock : public Investment {};//股票
class Bond : public Investment {};  //债券
class RealEstate : public Investment {}; //不动产

void makeLogEntry(Investment* pInvmt) {}

//工厂函数
template<typename... Ts>
auto makeInvestment(Ts&&... params) //返回unique_ptr智能指针
{
    //自定义deleter
    auto delInvmt = [](Investment* pInvmt) //父类指针
    {
        makeLogEntry(pInvmt);
        delete pInvmt; //delete父类指针，所有析构函数须声明为virtual
    };

    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

    if (1/*a Stock Object should be created*/) {
        pInv.reset(new Stock(std::forward<Ts>(params)...)); //原始指针无法隐式转为unique_ptr，使用reset重置所有权
    }
    else if (0/*a Bond Object should be created*/)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (0/*a RealEstate should be created*/)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }

    return pInv;
}


int main()
{
    //1. unique_ptr作为工厂函数的返回值。
    std::shared_ptr<Investment> sp =  makeInvestment();  //从std::unique_ptr转换到std::shared_ptr（从独占到共享的
                                                         //转换简单而高效） 

    //2. PImpl手法的测试
    Widget w;  //注意Widget的析构函数必须手动实现。否则，则当w析构时编译器会将默认的析构函数inline
               //到这里来，但由于include widget.h在inline动作之前，此时编译器看到的是非完整类型的
               //Impl类。因此Widget类中的unique_ptr析构时，delete前检查出是个非完整类指针，从而报错。
}