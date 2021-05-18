#include <iostream>

using namespace std;

// class Student{
//     public:
//         Student(){
//             count++;
//         }
//         ~Student(){}
//         static int count;
//         int getCount()
//         {
//             return count;
//         }
// };
// int Student::count = 0;
// int main()
// {

//     std::string s1 = "123456";
//     std::string s2 = s1;
//     s2.append("abc");
//     printf("s1:%s\n",s1.c_str());
//     printf("s2:%s\n",s2.c_str());
// }
#include <iostream>

using namespace std;

 

class A

{

public:

    A():m_ival(0){test();}

    virtual void func(){cout << m_ival << " "<<endl;}

    void test(){func();}

public:

    int m_ival;

};

 

class B : public A

{

public:

    B(){test();}

    virtual void func()

    {

        ++m_ival;

        cout << m_ival << " "<<endl;

    }

};

 

int main()

{

    A* p = new B;  //new B时，会先构造父类A，由于进入A构造函数之前，A的虚表己构造好。因此，调用A构造函数时，会通过查找其虚表，因此调用A::func。

                   //构造完父类后，开始构造子类B。在进入构造函数之前父类A的虚表会被B所修改和覆盖，因此会调用到B::func

                  

    p->test();     //多态，调用B::func

}