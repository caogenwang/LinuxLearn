#include <iostream>
using namespace std;


class Student{

public:
    Student(int _a):a(_a){
        cout<<"Student construct"<<endl;
    }
    virtual~Student(){
        cout<<"Student delete"<<endl;
    }
    void func0()
    {
        cout<<"I am a student func0"<<endl;
    }

    virtual void func1()
    {
        cout<<"I am a virtual func1"<<endl;
    }
    virtual void func2()
    {
        cout<<"I am a virtual func2"<<endl;
    }

    virtual void func3()
    {
        cout<<"I am a virtual func3"<<endl;
    }
    int a;
};

class Xiaoming:public Student
{
    public:
    Xiaoming(int a):Student(a){};
    ~Xiaoming(){}
    void func0()
    {
        cout<<"I am a Xiaoming func0"<<endl;
    }
};

class B{
    char b;
    int a;
};

int main()
{

    cout<<"size (B):"<<sizeof(B)<<endl;


    Student s1(10);
    Student s2(20);
    cout<<"size (studet):"<<sizeof(Student)<<endl;
    cout<<"size (int):"<<sizeof(int)<<endl;
    cout<<"size (char):"<<sizeof(char)<<endl;

    cout<<"size (studet*):"<<sizeof(Student*)<<endl;
    cout<<"size (int*):"<<sizeof(int*)<<endl;
    cout<<"size (char*):"<<sizeof(char*)<<endl;

    int *p1 = (int*)&s1;
    cout<<&s1<<endl;
    cout<<&(s1.a)<<endl;
    cout<<hex<<*p1<<endl;

    int *p2 = (int*)&s2;
    cout<<&(s2)<<endl;
    cout<<&(s2.a)<<endl;
    cout<<hex<<*p2<<endl;//每个类的虚函数列表的指针指向同一块内存
}

// typedef void(*Fun)(void); 
// int main()
// {

//     Student b(10);

//     Fun pFun = NULL;

//     cout << "虚函数表地址：" << (int*)(&b) << endl;//虚函数表在对象的开始位置
//     cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&b) << endl;//开始的虚函数指针存放应该是一个地址这个地址就是第一个虚函数的地址
//     // Invoke the first virtual function 

//     pFun = (Fun)*(int*)*(int*)(&b);//指向第一个函数的位置
//     pFun();
// }

// 通过这个示例，我们可以看到，我们可以通过强行把&b转成int *，取得虚函数表的地址，然后，再次取址就可以得到第一个虚函数的地址了，也就是Base::f()，这在上面的程序中得到了验证（把int* 强制转成了函数指针）。通过这个示例，我们就可以知道如果要调用Base::g()和Base::h()，其代码如下：

//             (Fun)*((int*)*(int*)(&b)+0);  // Base::f()

//             (Fun)*((int*)*(int*)(&b)+1);  // Base::g()

//             (Fun)*((int*)*(int*)(&b)+2);  // Base::h()


// int main()
// {
//     Student *s1 = new Xiaoming(10);
//     s1->func0();

// }