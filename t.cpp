#include <vector>
#include <iostream>
using namespace std;

class  A{
    virtual void c(){};
};
int main()
{
    int *p =(int*)malloc(10);
    cout<<sizeof(*p)<<endl;
}awk 'function sum(n,m){total=n+m;return total} BEGIN{print sum(1,2)}'