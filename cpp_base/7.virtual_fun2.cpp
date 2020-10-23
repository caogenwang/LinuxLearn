#include <iostream>
using namespace std;

void (*pFunc) (void);



class Student{

public:
    Student(int _a):a(_a){
        cout<<"Student construct"<<endl;
    }
    virtual~Student(){
        cout<<"Student delete"<<endl;
    }
    virtual void func()
    {
        cout<<"I am a virtual func"<<endl;
    }
    virtual void func2()
    {
        cout<<"I am a virtual func2"<<endl;
    }
    int a;
};

class Xiaoming:public Student
{
public:
    Xiaoming(int a,int _b):Student(a),b(_b)
    {
        cout<<"Xiaoming construct"<<endl;
    }
    virtual~Xiaoming(){
        cout<<"Xiaoming delete"<<endl;
    }
    virtual void func()
    {
        cout<<"I am a Xiaoming'virtual func"<<endl;
    }
    int b;
};

typedef void (Student::*fun)();
fun f = &Student::func;

// int main()
// {
//     /*我们先看在没有定义虚函数的情况下对象的大小*/
//     Student s1(10);
//     cout<<"Has virtual func size:"<<sizeof(s1)<<endl;//大小为什么是16？注意字节对齐,只存在一个虚函数指针，指向虚函数表
//     cout<<"Object addr:"<<&s1<<endl;       
//     cout<<"Member a's addr:"<<&(s1.a)<<endl;
    
//     // long * p = (long *)&s1;
//     // cout<<p<<endl;
//     // cout<<* p<<endl; 
//     return 0;
// }

template<class dst_type, class src_type>
dst_type pointer_cast(src_type src)
{
	return *static_cast<dst_type*>(static_cast<void*>(&src));
}

template<class dst_type, class src_type>
dst_type union_cast(src_type src)
{
	union
	{
		src_type s;
		dst_type d;
	} u;
 
	u.s = src;
	return u.d;
}


int main()
{

    Xiaoming x1(10,20);
    cout<<"Has virtual func size:"<<sizeof(x1)<<endl;//大小为什么是16
    cout<<"x1's addr:      "<<&x1<<endl;
    cout<<"Member a's addr:"<<&(x1.a)<<endl;
    cout<<"Member b's addr:"<<&(x1.b)<<endl;
    x1.func();//I am a Xiaoming'virtual func
    x1.func2();//I am a virtual func2

    Student *s1 = dynamic_cast<Student*>(&x1);
    cout<<"s1's addr:      "<<s1<<endl;
    cout<<"Member a's addr:"<<&(s1->a)<<endl;
    s1->func();//I am a Xiaoming'virtual func
    s1->func2();//I am a virtual func2


    cout<<"------------------"<<endl;
    (s1->*f)();

    cout<<"---------Student---------"<<endl;

    cout << pointer_cast<int*>(&Student::func) << endl;
    cout << pointer_cast<int*>(&Student::func2) << endl;
    cout<<"---------Xiaoming---------"<<endl;
    cout << pointer_cast<int*>(&Xiaoming::func) << endl;
    cout << pointer_cast<int*>(&Xiaoming::func2) << endl;

    cout<<"---------Student---------"<<endl;
    cout <<  union_cast<void*>(&Student::func) << endl;
    cout << union_cast<int*>(&Student::func2) << endl;

    cout<<"---------Xiaoming---------"<<endl;
    cout <<  union_cast<void*>(&Xiaoming::func) << endl;
    cout << union_cast<int*>(&Xiaoming::func2) << endl;

}
/*
这里主要要验证存在虚函数时候，虚函数列表是怎么布局的：在虚函数的列表中，如果子类没有重写父类的方法，则对应的位置是父类
的函数，如果重写了，则放置的是子类的对象的函数。从强制转换的结果来看，强制转换成的父类仍然调用的是子类重写的函数。

类中只存了一个虚函数列表指针。大小根据系统位数决定，4个字节或者8个字节。
*/


/*
C++取成员函数地址方法

一、通过静态转换
template<class dst_type, class src_type>
dst_type pointer_cast(src_type src)
{
	return *static_cast<dst_type*>(static_cast<void*>(&src));
}
*/ 

/*
二、通过联合体的共享存储机制

template<class dst_type, class src_type>
dst_type union_cast(src_type src)
{
	union
	{
		src_type s;
		dst_type d;
	} u;
 
	u.s = src;
	return u.d;
}
*/


/*
三、通过C语言的可变参数不检测参数类型（32位平台）
__declspec(naked) void* __cdecl cdecl_cast(...)
{
	__asm
	{
		mov eax, dword ptr[esp+4]
		ret
	}
}
*/

/*
四、通过汇编的offset语句取成员函数偏移得到地址（需要VS2012以上）
#define asm_cast(var, addr)	\
{							\
	__asm					\
	{						\
		mov var, offset addr\
	}						\	
}
*/

/*
测试代码
class A
{
public:
	void fun() {};
};
 
int main(int argc, char* argv[])
{
	A a;
	cout << std::hex << &a << '\n';
	
	cout << pointer_cast<void*>(&A::fun) << endl;
	cout <<  union_cast<void*>(&A::fun) << endl;
	cout <<  cdecl_cast(&A::fun) << endl;
 
	void* p = NULL;
	asm_cast(p, A::fun);
	cout << p << endl;
   
        return 0;
}

*/