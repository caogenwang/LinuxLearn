#include <iostream>
#include <functional>
 
using namespace std;
class Test
{
public:
    typedef int(*FunType)(int, int);//采用函数指针
    void Add(FunType fun,int a,int b)
    {
        int sum = fun(a, b);
        std::cout << "sum:" << sum << std::endl;
    }
};

class Test2
{
public:
    void Add(std::function<int(int, int)> fun, int a, int b)//绑定函数
    {
        int sum = fun(a, b);
        std::cout << "sum:" << sum << std::endl;
    }
};


int add(int a,int b)
{
    return a + b;
}

class TestAdd
{
public:
    int Add(int a,int b)
    {
        std::cout << "TestAdd::Add" << std::endl;
        return a + b;
    }
};



int  main()
{
    Test test;
    test.Add(add, 1, 2);

    Test2 test2;
    test2.Add(add,2,3);

    TestAdd testAdd;
    testAdd.Add(std::bind(&TestAdd::Add, testAdd, std::placeholders::_1, std::placeholders::_2), 1, 2);

    return 0;
}
