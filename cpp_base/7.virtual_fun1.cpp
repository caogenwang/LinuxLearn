#include <iostream>
using namespace std;

#pragma pack(1)//这里编译时候按照一个字节对齐，也就是取消字节对齐，得到的类的类的大小就是12
class Student{

public:
    Student(int _a):a(_a){}
    virtual~Student(){ }
    int a;
};

int main()
{
    /*我们先看在没有定义虚函数的情况下对象的大小*/
    Student s1(10);
    cout<<"Has virtual func size:"<<sizeof(s1)<<endl;//大小为什么是16？注意字节对齐
    cout<<"Object addr:"<<&s1<<endl;            //0x7ffee37fe518
    cout<<"Member a's addr:"<<&(s1.a)<<endl;    //0x7ffee37fe520
    return 0;
}

// 对齐是个比较难理解的问题，如果弄懂了一段时间后可能又忘了，不如直接取消对齐，相关配置代码如下：

// #pragma pack (n) // 编译器将按照n个字节对齐；
// #pragma pack() // 恢复先前的pack设置,取消设置的字节对齐方式
// #pragma pack(pop)// 恢复先前的pack设置,取消设置的字节对齐方式
// #pragma pack(1) // 按1字节进行对齐 即：不行进行对齐
// 所以只需要在includes中加最后一句即可，注意协议双方都需要取消字节对齐
