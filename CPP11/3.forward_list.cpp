#include <iostream>
#include <forward_list>
using namespace std;
int main()
{
    std::forward_list<int> numbers = {1,2,3,4,5,4,4};//初始化列表
    std::cout << "numbers:" << std::endl;
    for (auto number : numbers)
    {
        std::cout << number << std::endl;
    }
    numbers.remove(4);
    std::cout << "numbers after remove:" << std::endl;
    for (auto number : numbers)
    {
        std::cout << number << std::endl;
    }
    return 0;
}