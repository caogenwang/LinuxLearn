#include <iostream>
using namespace std;


int main2()
{
    int a = 100;
    int *p = &a;
    int &b=a;
    cout<<&a<<endl;
    cout<<++p<<endl;
    cout<<&p<<endl;
    cout<<b<<endl;
    cout<<&b<<endl;
    b = 200;
    cout<<a<<endl;
    cout<<sizeof(b)<<endl;
    cout<<sizeof(p)<<endl;

    char c = 'b';
    char *p2 = &c;
    char &d = c;
    cout<<c<<endl;
    cout<<sizeof(p2)<<endl;
    cout<<sizeof(c)<<endl;//引用的大小是和类型相关的。但是指针的大小与类型无关。
    return 0;
}
/*4、向任何一个未初始化的指针变量所指向的内容赋值是极其危险的，并且是不允许的。比如：*/
/*
6、注意区分* p++和(* p)++的区别：

* p++：因为运算符*和++的优先级相同，左结合性（从右向左）。先取出p的值，p再++；
(* p)++：因为运算符*和++的优先级相同，左结合性（从右向左）。先取出p的值，值再++。
注意：*（指针运算符）、&（取地址运算符）、++、--，这四个运算符的优先级都相同，且都是左结合性（从右向左）。
必须强调的是，用指针来访问数组元素时，编译程序不作下标是否越界的检查
*/

/*
对于函数使用引用类型的形参，实参直接使用变量名。而对于函数使用指针类型的实参，实参必须是变量的地址；
对于函数使用引用类型的形参，函数内直接使用变量名计算。而对于函数使用指针类型的实参，函数内需要使用“*”来取指针指向的数据。
*/

int& fun(void) {
	static int count;
	return ++count;
}
 
int main()
{
	cout << fun() << endl;
	fun() = 100;//将值直接付给了static变量
	cout << fun() << endl;
 
	system("pause");
	return 0;
}

/*
    18、在C++中当指定函数的类型为void时，表示其没有返回值，或者说返回值的值无效。
    当把指针定义为void类型时，表示可以指向任意类型的数据。void型指针也称为无类型指针，
    可以把任意类型的指针赋值给它。但若将void型的指针赋值给其他类型的指针变量时，则必须进行强制类型转换。
*/