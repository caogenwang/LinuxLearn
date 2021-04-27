#include<iostream>
#include<functional>
using namespace std;

class Minus{
    public:
        int operator()(int i,int j){
            return i - j;
        }
};

template<typename T>
class Minus2{
    public:
        T operator()(T t1,T t2){
            return t1 - t2;
        }
};

int main(){
    function<int(int,int)> g = Minus();
    cout<<g(10,5)<<endl;

    function<float(float,float)> g2 = Minus2<float>();
    cout<<g2(10.0,5.0)<<endl;
    return 0;
}