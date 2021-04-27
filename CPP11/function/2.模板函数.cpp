#include<iostream>
#include<functional>
using namespace std;

template<typename T>
T add(T t1,T t2)
{
    return t1 + t2;
}

int main()
{
    
    function<int(int,int)> g = add<int>;
    cout<<g(10,20)<<endl;
    return 0;
}