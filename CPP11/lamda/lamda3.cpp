#include <iostream>
#include<algorithm>
#include <functional>

using namespace std;
using namespace std::placeholders;

int g_ubound = 10;
vector<int> nums = { 8, 9, 10, 11, 12, 13, 14, 15, 16,17,18,19,20 };
vector<int> largeNums;

//显示vector中的元素
void print(vector<int>& vec)
{
    for (auto& elem : vec) {
        cout << elem << " ";
    }

    cout << endl;
}

//函数
inline void LargNumsFunc(int i)
{
    if (i > g_ubound)
    {
        largeNums.push_back(i);
    }
}

//仿函数
class LargeNums
{
private: 
    int ubound;
public:
    LargeNums(int u):ubound(u){}

    void operator()(int i) const
    {
        if (i > ubound)
        {
            largeNums.push_back(i);
        }
    }
};
void test(int ubound)
{
    //1. 使用传统的for(缺点：需要直接使用全局变量g_ubound)
    for (auto iter = nums.begin(); iter != nums.end(); ++iter) {
        if (*iter > ubound) {
            largeNums.push_back(*iter);
        }
    }
    print(largeNums);

    largeNums.clear();
    //2.使用函数指针
    //缺点：函数定义在别的地方，代码阅读不方便。inline非强制性的，内联不一定成功。可
    //      能导致性能问题。且LargeNumFunc由于使用了全局变量，是个有状态函数，函数重用性不高。
    for_each(nums.begin(), nums.end(), LargNumsFunc);
    print(largeNums);

    largeNums.clear();
    //3. 使用仿函数
    //优点：仿函数可以拥有状态，由于for_each第3个参数只能传递一个可调用对象而不能传递额外的参数。
    //      因此，利用仿函数就可以克服这一不足。
    //缺点：需要单独定义一个仿函数类。
    for_each(nums.begin(), nums.end(), LargeNums(g_ubound));
    print(largeNums);

    largeNums.clear();
    //4. 使用lambda表达式
    //优点：比仿函数书写上更简洁，代码的功能更清晰。
    for_each(nums.begin(), nums.end(), [ubound](int i) 
    {
            if (i > ubound) {
                largeNums.push_back(i);
            }
    });
    print(largeNums);
}
int main()
{
    //1. lambda可简化标准库的调用(统计(50,73]之间的元素个数)
    vector<int> v{ 15, 37, 94, 50, 73, 58, 28, 98 };
    //1.1 组合使用bind
    auto f1 = std::bind(std::logical_and<bool>(),
                        std::bind(std::greater<int>(), _1, 50),
                        std::bind(std::less_equal<int>(), _1, 73)
                        );
    cout << count_if(v.begin(), v.end(), f1) << endl; //2

    //1.2 使用lambda表达式
    auto f2 = [](int x) {return (50<x) && (x<=73); };
    cout << count_if(v.begin(), v.end(), f2) << endl; //2
    int cnt = count_if(v.begin(), v.end(), [](int x)
              {
                 return  (50 < x) && (x <= 73);
              });
    cout << cnt << endl; //2

    //2. lambda与其他可调用对象使用上的比较
    test(g_ubound);

    return 0;
}
/*输出结果
2
2
2
11 12 13 14 15 16 17 18 19 20
11 12 13 14 15 16 17 18 19 20
11 12 13 14 15 16 17 18 19 20
11 12 13 14 15 16 17 18 19 20
*/