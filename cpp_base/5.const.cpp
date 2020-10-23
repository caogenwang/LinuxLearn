#include <iostream>
using namespace std;

// 用const定义的标识符常量时，一定要对其进行初始化。在说明时进行初始化是对这种常量置值的唯一方法，
//不能用赋值运算符对这种常量进行赋值。

int main(int argc,char**argv)
{
    const int a = 10;
    const int c = 11;
    const int &b = a;

    int d = 20;
    int e = 21;
    const int *p = &d;//这种形式定义的指针变量表示指针变量指向的数据是一个常量,指针可以指向不同的地址，但是不能修改对应地址内的值
    p = &e;           //等价于int const *p

    int * const p2 = &d;
    *p2 = 200;//指针是const的，不能指向其他地址，但是可以修改对应地址的值


    // 第三种形式是把一个const放在指针变量的类型前面，再把另一个const放在指针变量的“*”之后
    int f;
    const int* const p3 = &f;
    // *p3 = 100;
    // p3 = &e;

    /*
        这种形式定义的指针变量，表示指针变量的值是一个常量，指针变量所指向的数据也是一个常量。
        因此不能改变指针变量的值，也不能改变指针变量所指向的数据值。这种形式定义的指针变量，在定义时必须赋初值.
    */

}

