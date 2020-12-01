#include <iostream>
#include <numeric>
#include <functional>

using namespace std;

class A 
{
public:
    A():b(0){
        c.a = 1;
    }
    /*包含类类型*/
    typedef struct foo_s{
        int a;
    }foo_t;
    int b;
    foo_t c;
};

template<typename T>
class Test{
public:
    Test(T __t):t(__t),item(__t.c){
        cout<<"construct func\n";
    }
    bool operator()(T &t1,T &t2){
        return t1.a > t2.b;
    }
    typedef typename T::foo_t foo_type;
    foo_type item;//foo_type这里代表了一个foo类型的指针
    T t;
};


int main()
{
    A a;
    Test<A> t(a);
    cout<<t.t.b<<endl;
    cout<<t.item.a<<endl;
    return 0;
}