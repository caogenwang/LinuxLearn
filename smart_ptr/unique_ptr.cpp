#include <iostream>
#include <memory>

using namespace std;

class A{
    A(int _i):i(_i){}
    int i;
};
int main()
{
    unique_ptr<A> pa(new A(9));
}