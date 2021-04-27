#include<iostream>
#include<functional>
using namespace std;

class Minus{
    public:
        int add(int i,int j){
            return i + j;
        }
};

int main()
{
    Minus m;
    function<int(int,int)> g = bind(&Minus::add,&m,placeholders::_1, placeholders::_2);
     cout << g(1, 2) << endl;
    return 0;
}