#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <string>
using namespace std;
struct X
{
    void foo(int,std::string const&);
    std::string bar(std::string const&);
};
X x;
auto f1 = std::async(&X::foo,&x,42,"hello");
auto f2 = std::async(&X::bar,&x,"goobye");

struct Y
{
    double operator()(double);
};
Y y;
auto f3 = std::async(Y(),3.14);
auto f4 = std::async(std::ref(y),2.718);

X bar

int main ()
{

}



