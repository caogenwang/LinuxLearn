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