#include <iostream>
 
using namespace std;
 
class A
{
public:
	A(){};
	~A(){};
	virtual void vfun(){cout<<"vfun called!"<<endl;}
};

int main()
{
	A *a = new A();
	long vbaddr=*(int *)a;   //虚函数表地址
	cout<<"addr of vb : "<<hex<<vbaddr<<endl;
	long vfaddr= *(int *)&vbaddr;   //虚函数vfun地址
	cout<<"addr of vfun : "<<hex<<vfaddr<<endl;
	

	// ((void(*)(void))&vfaddr)();   //根据虚函数地址调用虚函数
 
	delete a;
	return 0;
}