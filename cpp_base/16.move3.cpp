#include <iostream>

using namespace std;

class A{
public:
    A(){
        cout<<"A construct "<<this<<endl;
    }
    ~A(){
        cout<<"A delete "<<this<<endl;
    }
    A(A&&) {
        cout<<"A(A&&) "<<this<<endl;
    }
    A&operator=(const A&a)
    {
        cout<<"operator=(A&a) "<<this<<endl;
        return const_cast<A&>(a);
    }

    A& operator=(A&&) {
        cout<<"operator=(A&&) "<<this<<endl;
        return *this;
    }
private:
    int x;
};


int main()
{
    A a;
    A aa(std::move(a));//移动构造
    A b;
    A c;
    a = b;//普通赋值
    c = std::move(a);//移动赋值
    return 0;
}