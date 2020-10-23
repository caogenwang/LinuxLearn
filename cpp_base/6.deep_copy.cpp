#include <iostream>
using namespace std;

struct Child{
    int a;
    int b;
};
class Student{

public:
    Student(int _a,Child*_c):a(_a),p(_c){}
    ~Student(){
        if(p)
        {
            delete p;
            cout<<"Student delete"<<endl;
        }
    }
    Student(const Student &s1)
	{
        if(this != &s1 && s1.p)
        {
            this->p = new Child();
            this->p->a = s1.p->a;
            this->p->b = s1.p->b;
        }
        this->a = s1.a;
        cout<<"copy func"<<endl;
	}

    Student& operator =(const Student& s1)
    {
        if(this != &s1 && s1.p)
        {
            this->p = new Child();
            this->p->a = s1.p->a;
            this->p->b = s1.p->b;
        }
        this->a = s1.a;
        cout<<"= func"<<endl;        
        return *this;
    }
    Child * p;
private:
    int a;
    
};

// int main()
// {
//     Student s1(10);
//     s1.p = (child*)malloc(sizeof(child));
//     s1.p->a = 100;
//     Student s2(s1);
//     Student s3(20);

//     cout<<&s1<<endl;
//     cout<<&s2<<endl;
//     cout<<&s3<<endl;

//     cout<<s1.p<<endl;
//     cout<<s2.p<<endl;
//     cout<<s3.p<<endl;//如果不定义自己的拷贝和赋值构造函数，
//                     //则类中的指针会指向同一块地址，一旦其中一个地址释放，则其他指针变为悬垂指针

//     return 0;
// }

int main()
{

    Child *chi1 = new Child();
    Student s1(10,chi1);
    cout<<chi1<<endl;
    cout<<&s1<<endl;

    Child *chi2 = new Child();
    Student s2(20,chi2);
    cout<<chi2<<endl;
    cout<<&s2<<endl;

    s2 = s1;
    cout<<s2.p<<endl;


    return 0;
}

/*
    在对象拷贝过程中，如果没有自定义拷贝构造函数，系统会提供一个缺省的拷贝构造函数，
    缺省的拷贝构造函数对于基本类型的成员变量，按字节复制，对于类类型成员变量，调用其相应类型的拷贝构造函数
    加入对象中存在一个指针变量指向堆的一块内存，则复制的对象中的指针也会指向同一块堆内存。当原始对象销毁后，
    复制对象再销毁空间时，会导致同一块堆内存重复释放。

    解决方法就是拷贝构造函数和赋值构造函数都要自己实现，对象内的指针要进行深拷贝，新开辟一段内存空间。
*/