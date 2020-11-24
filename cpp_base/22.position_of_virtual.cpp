#include <iostream>
#include <string>
extern "C"{
    #include <stdio.h>
}
using namespace std;
/*虚函数指针在对象内存布局的最开始四个字节就是一个虚函数表指针
（32位编译器），而这个指针变量的值自然就是虚函数表的地址了*/

class A
{
public:
	A(){};
	~A(){};
	virtual void vfun(){cout<<"vfun called!"<<endl;}
};

int main()
{
    A *a = new A();//a指向堆上的一块空间，
    printf("a:%x\n",a);
    printf("%x\n",&a);
    long vbaddr=*(int *)a;
 
	delete a;
	return 0;
}

