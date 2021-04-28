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