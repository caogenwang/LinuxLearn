#include<iostream>
#include<functional>
using namespace std;

int plus(int a,int b){
    return a + b;
}

int main()
{

    function<int(int,int)> f = plus;
    cout<<f(10,20)<<endl;
    return 0;
}