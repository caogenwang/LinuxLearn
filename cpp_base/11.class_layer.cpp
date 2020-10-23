#include <iostream>
using namespace std;

// #pragma pack(1) 
class  A{
public:
    A(void*ptr=nullptr){
        cout<<"A"<<endl;
    }
    char *a;
};

struct C:public A{
    int c;
};

class D{
public:
    D(){}
    ~D(){}
    A getA()
    {
        return a;
    }
    A a{nullptr};//注意在类的成员函数都是指针时，可以如此定义
};

int *my_malloc(int size)
{
    int *p = (int*)malloc(sizeof(int)*size);
    if(p == nullptr)
        return nullptr;
    cout<<p<<endl;
    memset(p,0x0,sizeof(int)*size);
    return p;
}

// int main()
// {
//     //指针不能修改常量字符串的内容
//     char *ptr = "abcdef";
//     cout<<ptr[4]<<endl;
//     ptr[4] = 'k';
//     // cout<<ptr[4]<<endl;//Bus error: 10
// }
int main()
{
    D *d;
    d->getA();
    return 0;
}