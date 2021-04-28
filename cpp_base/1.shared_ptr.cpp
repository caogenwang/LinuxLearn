
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

int main()
{
    shared_ptr<Student> ptr1 = shared_ptr<Student>(new Student(18,4,0));//与unique_ptr不同的地方
    cout<<ptr1->age<<endl;

    shared_ptr<Student> ptr2 = ptr1;
    shared_ptr<Student> ptr4 = ptr1;
    cout<<ptr2->age<<endl;
    cout<<ptr1.use_count()<<endl;// 3

    // Student*s1 = new Student(19,4,0);
    // shared_ptr<Student> ptr3(s1);
    // cout<<ptr3.use_count() <<endl;

    // shared_ptr<Student> ptr5 = ptr3;
    // cout<<ptr5.use_count() <<endl;
    // cout<<ptr3.use_count() <<endl;
    // cout<<ptr3.get()<<endl;//0x7fbcdfd027b0
    return 0;
}

/*
    shared_ptr基本用法
    智能指针的行为类似于常规指针，重要的区别是它负责自动释放所指向的对象。
    新标准提供的两种重要的智能指针的区别在于管理底层指针的方式：
    shared_ptr允许多个指针指向同一个对象；unique_ptr则独占所指向的对象。

    shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存。
    每使用它一次，内部的引用计数加1，每析构一次，内部的引用计数减1，减为0时，
    释放所指向的堆内存。shared_ptr内部的引用计数是安全的，但是对象的读取需要加锁。
*/ 

// 初始化方式
// shared_ptr有如下几种初始化方式：

// 裸指针直接初始化，但不能通过隐式转换来构造，因为shared_ptr构造函数被声明为explicit；
// 允许移动构造，也允许拷贝构造；
// 通过make_shared构造，在C++11版本中就已经支持了。

// #include <iostream>
// #include <memory>

// class Frame {};

// int main()
// {
//   std::shared_ptr<Frame> f(new Frame());              // 裸指针直接初始化
//   std::shared_ptr<Frame> f1 = new Frame();            // Error，explicit禁止隐式初始化
//   std::shared_ptr<Frame> f2(f);                       // 拷贝构造函数
//   std::shared_ptr<Frame> f3 = f;                      // 赋值构造函数
//   f2 = f;                                             // copy赋值运算符重载
//   std::cout << f3.use_count() << " " << f3.unique() << std::endl;

//   std::shared_ptr<Frame> f4(std::move(new Frame()));        // 移动构造函数
//   std::shared_ptr<Frame> f5 = std::move(new Frame());       // Error，explicit禁止隐式初始化
//   std::shared_ptr<Frame> f6(std::move(f4));                 // 移动构造函数
//   std::shared_ptr<Frame> f7 = std::move(f6);                // 移动构造函数
//   std::cout << f7.use_count() << " " << f7.unique() << std::endl;

//   std::shared_ptr<Frame[]> f8(new Frame[10]());             // Error，管理动态数组时，需要指定删除器
//   std::shared_ptr<Frame> f9(new Frame[10](), std::default_delete<Frame[]>());

//   auto f10 = std::make_shared<Frame>();               // std::make_shared来创建

//   return 0;
// }

// 可以看出，shared_ptr和unique_ptr在初始化的方式上就有一些差别：
// 尽管两者都不支持隐式初始化，但是unique_ptr不支持拷贝构造和拷贝赋值，而shared_ptr都支持。除此之外，
// 由于删除器不是shared_ptr类型的组成部分，在管理动态数组的时候，shared_ptr需要指定删除器。

/*
    删除器可以是普通函数、函数对象和lambda表达式等。默认的删除器为std::default_delete，
    其内部是通过delete来实现功能的。
    与unique_ptr不同，删除器不是shared_ptr类型的组成部分。也就是说，两个shared_ptr，
    就算sp1和sp2有着不同的删除器，但只要两者的类型是一致的，
    都可以被放入vector<shared_ptr >类型的同一容器里。例如：
*/ 

// #include <iostream>
// #include <memory>
// #include <vector>

// class Frame {};

// int main2()
// {
//     auto del = [](Frame* f){
//     std::cout << "delete1" << std::endl;
//     delete f;
//     };

//     auto del1 = [](Frame* f){
//     std::cout << "delete1" << std::endl;
//     delete f;
//     };
//     auto del2 = [](Frame* f){
//     std::cout << "delete2" << std::endl;
//     delete f;
//     };

//     std::shared_ptr<Frame> f1(new Frame(), del1);
//     std::shared_ptr<Frame> f2(new Frame(), del2);
//     std::unique_ptr<Frame, decltype(del)> f3(new Frame(), del);

//     std::vector<std::shared_ptr<Frame> > v;
//     v.push_back(f1);
//     v.push_back(f2);
//     v.push_back(f3);
//   return 0;
// }
// 可以很明显地看出，unique_ptr需要指定原指针的指向类型，
// 还需要指定删除器类型；但shared_ptr只需要指定原指针的指向类型即可。
// 与std::unique_ptr不同，自定义删除器不会改变std::shared_ptr的大小。其始终是祼指针大小的两倍。
// 常用操作
// s.get()：返回shared_ptr中保存的裸指针；
// s.reset(…)：重置shared_ptr；
// s.use_count()：返回shared_ptr的强引用计数；
// s.unique()：若use_count()为1，返回true，否则返回false。

// class Frame std::enable_shared_from_this<Frame>{
//   public:
//     std::shared_ptr<Frame> GetThis() {
//       return std::shared_ptr<Frame>(this);
//     }
// };在from this中这种调用方法会出错

// class Frame : public std::enable_shared_from_this<Frame> {
//   public:
//     std::shared_ptr<Frame> GetThis() {
//       return shared_from_this();
//     }
// };

// int main()
// {
//   std::shared_ptr<Frame> f1(new Frame());
//   std::shared_ptr<Frame> f2 = f1->GetThis();
//   std::cout << f1.use_count() << " " << f2.use_count() << std::endl;

//   std::shared_ptr<Frame> f3(new Frame());
//   std::shared_ptr<Frame> f4 = f3;
//   std::cout << f3.use_count() << " " << f4.use_count() << std::endl;
  
//   return 0;
// }

// template<typename _Tp>
// class enable_shared_from_this
// {
//   protected:
//     constexpr enable_shared_from_this() noexcept { }
//     enable_shared_from_this(const enable_shared_from_this&) noexcept { }
//     enable_shared_from_this& operator=(const enable_shared_from_this&) noexcept { return *this; }
//     ~enable_shared_from_this() { }

//   public:
//     shared_ptr<_Tp> shared_from_this()
//     { return shared_ptr<_Tp>(this->_M_weak_this); }

//     shared_ptr<const _Tp> shared_from_this() const
//     { return shared_ptr<const _Tp>(this->_M_weak_this); }

//   private:
//     template<typename _Tp1>
//     void _M_weak_assign(_Tp1* __p, const __shared_count<>& __n) const noexcept
//     { _M_weak_this._M_assign(__p, __n); }

//     template<typename _Tp1, typename _Tp2>
//     friend void __enable_shared_from_this_helper(const __shared_count<>&,
//             const enable_shared_from_this<_Tp1>*,
//             const _Tp2*) noexcept;

//     mutable weak_ptr<_Tp>  _M_weak_this;
// };
