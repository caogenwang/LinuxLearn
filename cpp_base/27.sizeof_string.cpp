#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s;
    cout<<"size:"<<sizeof(s)<<endl;//24，内部实现就是三条指针

    string s1("hello");
    cout<<"size:"<<sizeof(s1)<<endl;//24

    cout<<s1[0]<<endl;//h，如何实现的[]符号的重载,指针的移动
    // cout<<s1.at(10)<<endl;//超出界限，抛出异常terminating with uncaught exception of type std::out_of_range: basic_string
}

/*
const_reference operator[](size_type __n) const
    { return *(_M_start + __n); }
reference operator[](size_type __n)
    { return *(_M_start + __n); }
*/
