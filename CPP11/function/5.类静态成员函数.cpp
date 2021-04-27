#include<iostream>
#include<functional>
using namespace std;

class Minus{
    public:
        static int add(int i,int j){
            return i + j;
        }
};

int main()
{
    function<int(int,int)> g = &Minus::add;
    cout<<g(10,20)<<endl;
    return 0;
}