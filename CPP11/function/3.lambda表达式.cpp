#include<iostream>
#include<functional>
using namespace std;

auto minus = [](int i,int j){return i - j;};

int main(){
    function<int(int,int)> g = minus;
    cout<<g(10,5)<<endl;
    return 1;
}