#include <iostream>
using namespace std;

const int *Search(const int *a,int n,int val);

int main(){
    int a[10] = {0,1,2,3,4,5,6,7,8,9};
    int val = 5;

    int *p;
    p = const_cast<int*>(Search(a,10,val));
    *p = 50;//Bus error: 10
    cout<<a[5]<<endl;
    // 在例5中我们定义了一个函数，用于在a数组中寻找val值，
    // 如果找到了就返回该值的地址，如果没有找到则返回NULL。
    // 函数Search返回值是const指针，当我们在a数组中找到了val值的时候，
    // 我们会返回val的地址，最关键的是a数组在main函数中并不是const，
    // 因此即使我们去掉返回值的常量性有可能会造成a数组被修改，但是这也依然是安全的。
    if(p == NULL)
        cout<<"Not found the val in array a"<<endl;
    else
        cout<<"have found the val in array a and the val = "<<*p<<endl;
    return 0;
}
const int * Search(const int * a, int n, int val)//希望返回的这个地址对应的值不会被修改，上一个因为a本身是常量，所以不会被修改，但是数组声明常量会常量bus err
{
    int i;
    for(i=0; i<n; i++)
    {
        if(a[i] == val)
            return &a[i];
    }
    return  NULL;
}

// 二、放在函数前后区别
// 1、int GetY() const;

// 2、const int * GetPosition();

// 对于1
// 该函数为只读函数，不允许修改其中的数据成员的值。

// 对于2
// 修饰的是返回值，表示返回的是指针所指向值是常量。

// 　　int f() const;//const是修饰隐藏的this指针的，意思是类的成员变量是const，不可修改

// 　　const int f();//这个很简单，返回值是const类型的。

// 1. type funtion（type2 v）const;

// 　　等价于：type function(const *this, type2 v)const;

// 　　类的成员函数有隐含的this指针，所以上面的函数其实是有两个形参的，第一个就是this指针，函数后面的const 就是限定this指针为const，从而不允许函数修改调用该成员函数的类对象。

// 2.在函数前面有const的一般是在要求成员函数返回值为类对象的情形。

// 　　const classType& function(type2 v)const;

// 　　因为函数后面有const,从而这是一个const成员函数，而有规定const成员函数不能返回指向类对象的普通引用，因此函数的返回类型也必须是const 引用类型。