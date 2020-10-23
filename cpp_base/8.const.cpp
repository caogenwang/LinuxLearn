
// 类中可以定义const成员变量 
// const成员变量必须在初始化列表中指定初值 
//  const成员变量为只读变量 
// const关键字能够修饰对象，得到只读对象 
// 只读对象只能调用const成员函数 

#include <iostream>
using namespace std;

class Student{

public:
    Student(int _a,int _b):a(_a),b(_b){}
    ~Student(){}
private:
    int a;
    const int b;
    static int c;
    static const int d = 1;
};

int Student::c = 1;

/*
类中的const成员会被分配空间的 
类中的const成员变量的本质是只读变量 
类中的const成员变量只能在初始化列表中指定初始值 (引用数据成员也是)
编译器无法直接得到const成员的初始值，因此无法进入符号表成为真正意义上的常量。
*/

/*
const关键字能够修饰对象 
const修饰的对象为只读对象 
只读对象的成员变量不允许被改变 
只读对象是编译阶段的概念，运行时无效 
*/

/*
const对象只能调用const的成员函数 
const成员函数中只能调用const成员函数 
const成员函数中不能直接改写成员变量的值
const成员函数的定义：Type ClassName::function(Type p) const 
类中的函数声明与实际函数定义中都必须带const关键字
*/