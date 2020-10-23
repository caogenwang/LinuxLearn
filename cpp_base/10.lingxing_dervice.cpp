#include <iostream>
using namespace std;
 
//祖先类
 
class R
{
private:
	int r;
public:
	R(int i):r(i){};
	void print()
	{
		cout<<"r="<<r<<endl;
	}
};
 
//父类A
class A : virtual public R
{
private:
	int a;
public:
	A(int int1,int int2):R(int2),a(int1){};
};
 
//父类B
class B: virtual public R
{
private:
	int b;
public:
	B(int int1,int int2):R(int2),b(int1){};
};
 
//子类C
class C:public A,public B
{
private:
	int c;
public:
	// C(int int1,int int2,int int3):A(int2,int3),B(int2,int3),c(int1){};
    C(int int1,int int2,int int3,int int4):R(int1),A(int2,int1),B(int3,int1),c(int4){};
};
 
int main()
{
	R rrr(10);
	A aaa(20,30);
	B bbb(40,50);
	C ccc(60,70,80,90);
	rrr.print();
	aaa.print();
	bbb.print();
	ccc.print();//此时可以正确运行，C中的r和print直接从祖先类中继承，此时只有一份祖先类的成员，不会出现二义性
	return 0;
}