#include <iostream>
#include <memory>
using namespace std;

class Student{
    public:
        Student(int _age,int _grade,int _male):age(_age),grade(_grade),male(_male)
        {}
        ~Student(){
            cout<<"Student delete"<<endl;
        }
        void setAge();
        void setGrade();
        void setMale();

        int age;
        int grade;
        int male;
};

class Student2{
    public:
        Student2(int _age,int _grade,int _male,Student*_s):age(_age),
        grade(_grade),male(_male),uStu(_s)
        {}
        ~Student2(){
            cout<<"Student2 delete"<<endl;
        }
        int age;
        int grade;
        int male;
        unique_ptr<Student> uStu;//自动析构
        // Student *uStu;//需要手动析构
};

int main()
{
    // 1.unique_ptr<Student> pStu = std::unique_ptr<Student>(new Student(18,4,0));
    unique_ptr<Student> pStu(new Student(18,4,0));
    cout<<"---------pStu------------"<<endl;
    // pStu.reset();重置，对象会被析构掉
    // Student* pStu2 = pStu.release();//pStu是去控制权，但是不析构对象.这个时候需要主动删除对象，因为是在堆地址// delete pStu2;
    cout<<pStu->age<<endl;//0x7f9184402c10
    
    unique_ptr<Student> pStu2(std::move(pStu));//当需要转移控制权时，需要使用move语义
    cout<<pStu2->age<<endl;
    // cout<<pStu->age<<endl;//访问会出错


    {
        Student *s1 = new Student(18,4,0);
        unique_ptr<Student2> pst2 = unique_ptr<Student2>(new Student2(19,4,0,s1));
        cout<<pst2->age<<endl;
        cout<<pst2->uStu->age<<endl;
    }
    return 0;
}

/*
unique_ptr是C++11提供的用于防止内存泄漏的智能指针中的一种实现，独享被管理对象指针所有权的智能指针。unique_ptr对象包装一个原始指针，
并负责其生命周期。当该对象被销毁时，会在其析构函数中删除关联的原始指针。
转移一个unique_ptr将会把所有权也从源指针转移给目标指针(源指针被置空)。拷贝一个unique_ptr将不被允许
unique_ptr具有->和*运算符重载符，因此它可以像普通指针一样使用。

智能指针unique_ptr,提供了一种拥有唯一控制权的指针类，可以防止内存资源的泄漏，他控制权的移动需要使用移动语义，
一旦失去对一个指针的拥有权，那么这个指针类将不能再访问该对象。

*/

// #include <iostream>
// #include <memory>

// class Frame {};

// int main1()
// {
//   std::unique_ptr<Frame> f(new Frame());              // 裸指针直接初始化
//   std::unique_ptr<Frame> f1 = new Frame();            // Error，explicit禁止隐式初始化
//   std::unique_ptr<Frame> f2(f);                       // Error，禁止拷贝构造函数
//   std::unique_ptr<Frame> f3 = f;                      // Error，禁止拷贝构造函数
//   f1 = f;                                             // Error，禁止copy赋值运算符重载

//   std::unique_ptr<Frame> f4(std::move(new Frame()));      // 移动构造函数
//   std::unique_ptr<Frame> f5 = std::move(new Frame());     // Error，explicit禁止隐式初始化
//   std::unique_ptr<Frame> f6(std::move(f4));               // 移动构造函数
//   std::unique_ptr<Frame> f7 = std::move(f6);              // move赋值运算符重载

//   std::unique_ptr<Frame[]> f8(new Frame[10]());       // 指向数组

//   auto f9 = std::make_unique<Frame>();                // std::make_unique来创建，C++14后支持

//   return 0;
// }


// #include <iostream>
// #include <memory>

// class Frame {};

// void fun(std::unique_ptr<Frame> f) {}

// std::unique_ptr<Frame> getfun() {
//   return std::unique_ptr<Frame>(new Frame());       // 右值，被移动构造
//                                                     // 就算不是右值，也会被编译器RVO优化掉
// }

// int main()
// {
//   std::unique_ptr<Frame> f1(new Frame());
//   Frame* f2 = new Frame();
//   fun(f1);                    // Error，禁止拷贝构造函数
//   fun(f2);                    // Error，explit禁止隐式转换
//   fun(std::move(f1));         // 移动构造函数

//   std::unique_ptr<Frame> f3 = getfun();       // 移动构造函数

//   return 0;
// }

// 删除器
// 根据unique_ptr的模板类型来看：

// template <typename _Tp, typename _Dp = default_delete<_Tp> >
// class unique_ptr {...}

// 模板参数上，前者为unique_ptr需要关联的原始指针的类型，后者为删除器，默认值为default_delete。也就是说，删除器是unique_ptr类型的组成部分，可以是普通函数指针或lambda表达式。
// 注意，当指定删除器时需要同时指定其类型，即_Dp不可省略，可通过decltype获得
// 删除器的作用就是规定：当unique_ptr对象被销毁时，在其析构函数中释放关联的原始指针的方式。
// 一般情况下，都是通过delete进行释放操作。也就是说，一般情况下，不需要进行指定，使用默认的即可。例如：

// #include <iostream>
// #include <memory>

// class Frame {};

// void myDeleter(Frame* p)
// {
//   std::cout << "invoke deleter(Frame*)"<< std::endl;
//   delete p;
// }

// int main()
// {
//   std::unique_ptr<Frame, decltype(&myDeleter)> f1(new Frame(), myDeleter);
//   auto del = [](Frame* p) {//   del是一个lamdba表达式
//     std::cout << "invoke deleter([](Frame *))"<< std::endl;
//     delete p;
//   };
//   std::unique_ptr<Frame, decltype(del)> f2(new Frame(), del);

//   return 0;
// }

/*
    使用默认的删除器时，unique_ptr对象和原始指针的大小是一样的。当自定义删除器时，如果删除器是函数指针，则unique_ptr对象的大小为8字节。对于函数对象的删除器，unique_ptr对象的大小依赖于存储状态的多少，
    无状态的函数对象(如不捕获变量的lambda表达式)，其大小为4字节。

    常用操作
    u.get()：返回unique_ptr中保存的裸指针；
    u.reset(…)：重置unique_ptr；
    u.release()：放弃对指针的控制权，返回裸指针，并将unique_ptr自身置空。需要注意，此函数放弃了控制权但不会释放内存，如果不获取返回值，就丢失了指针，造成内存泄露；
    u.swap(…)：交换两个unique_ptr所指向的对象。

    额外需要注意的是：尽管unique_ptr禁止了拷贝构造和拷贝赋值，但是，nullptr是可以用来赋值的：

    u = nullptr;       //释放u所指向的对象，将u置为空
    u.reset(nullptr);    // u置为空

    特点
    与auto_ptr相比unique_ptr有如下特点：

    unique_ptr是一个独享所有权的智能指针，无法进行复制构造、copy赋值操作，只能进行移动操作。无法使两个unique_ptr指向同一个对象；
    unique_ptr智能指向一个对象，如果当它指向其他对象时，之前所指向的对象会被摧毁；
    unique_ptr对象会在它们自身被销毁时使用删除器自动删除它们管理的对象；
    unique_ptr支持创建数组对象方法
*/

// 指向单个类型对象
// template <typename _Tp, typename _Dp = default_delete<_Tp> >
// class unique_ptr
// {
//   class _Pointer {};

//   typedef std::tuple<typename _Pointer::type, _Dp>  __tuple_type;
//   __tuple_type                                      _M_t;

//   public:
//     typedef typename _Pointer::type   pointer;
//     typedef _Tp                       element_type;
//     typedef _Dp                       deleter_type;

//     constexpr unique_ptr() noexcept : _M_t()
//     { static_assert(!is_pointer<deleter_type>::value,
//         "constructed with null function pointer deleter"); }

//     explicit unique_ptr(pointer __p) noexcept : _M_t(__p, deleter_type())     // 裸指针构造函数，explicit阻止隐式构造
//     { static_assert(!is_pointer<deleter_type>::value,
//         "constructed with null function pointer deleter"); }

//     unique_ptr(unique_ptr&& __u) noexcept                 // 移动构造函数
//     : _M_t(__u.release(), std::forward<deleter_type>(__u.get_deleter())) { }

//     ~unique_ptr() noexcept                                // 析构函数
//     {
//       auto& __ptr = std::get<0>(_M_t);
//       if (__ptr != nullptr)
//         get_deleter()(__ptr);
//       __ptr = pointer();
//     }

//     unique_ptr& operator=(unique_ptr&& __u) noexcept      // move赋值运算符重载
//     {
//       reset(__u.release());
//       get_deleter() = std::forward<deleter_type>(__u.get_deleter());
//       return *this;
//     }

//     typename add_lvalue_reference<element_type>::type operator*() const   // 解引用
//     {
//       _GLIBCXX_DEBUG_ASSERT(get() != pointer());
//       return *get();
//     }

//     pointer operator->() const noexcept                   // 智能指针->运算符
//     {
//       _GLIBCXX_DEBUG_ASSERT(get() != pointer());
//       return get();
//     }

//     pointer get() const noexcept                          // 获得裸指针
//     { return std::get<0>(_M_t); }

//     deleter_type& get_deleter() noexcept                  // 获取删除器
//     { return std::get<1>(_M_t); }

//     explicit operator bool() const noexcept               // 类型转换函数，用于条件语句，如if(uniptr)之类
//     { return get() == pointer() ? false : true; }

//     pointer release() noexcept                            // 释放指针
//     {
//       pointer __p = get();
//       std::get<0>(_M_t) = pointer();
//       return __p;
//     }

//     void reset(pointer __p = pointer()) noexcept          // 重置指针
//     {
//       using std::swap;
//       swap(std::get<0>(_M_t), __p);
//       if (__p != pointer())
//         get_deleter()(__p);
//     }

//     void swap(unique_ptr& __u) noexcept                   // 交换指针
//     {
//       using std::swap;
//       swap(_M_t, __u._M_t);
//     }

//     unique_ptr(const unique_ptr&) = delete;               // 禁止拷贝构造函数
//     unique_ptr& operator=(const unique_ptr&) = delete;    // 禁止copy赋值运算符重载
// };

// // 指向数组类型
// template<typename _Tp, typename _Dp>
// class unique_ptr<_Tp[], _Dp>
// {
//   ...           // 与上文代码类似，省略

//   public:
//     typename std::add_lvalue_reference<element_type>::type operator[](size_t __i) const     // 数组[]操作符
//     {
//       _GLIBCXX_DEBUG_ASSERT(get() != pointer());
//       return get()[__i];
//     }
// }

/*
unique_ptr的构造函数被声明为explicit，禁止隐式类型转换的行为。可避免将一个普通指针传递给形参为智能指针的函数。假设，如果允许将裸指针传给void foo(std::unique_ptr<T>)函数，则在函数结束后会因形参超出作用域，裸指针将被delete的误操作；
unique_ptr的拷贝构造和拷贝赋值均被声明为delete。因此无法实施拷贝和赋值操作，但可以移动构造和移动赋值；
删除器是unique_ptr类型的一部分。默认为std::default_delete，内部是通过调用delete来实现；
unique_ptr可以指向数组，并重载了operator []运算符。
*/