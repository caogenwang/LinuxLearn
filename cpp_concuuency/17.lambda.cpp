#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
 
// bool cmp(int a, int b)
// {
//     return  a < b;
// }
 
// int main()
// {
//     vector<int> myvec{ 3, 2, 5, 7, 3, 2 };
//     vector<int> lbvec(myvec);
 
//     sort(myvec.begin(), myvec.end(), cmp); // 旧式做法
//     cout << "predicate function:" << endl;
//     for (int it : myvec)
//         cout << it << ' ';
//     cout << endl;
 
//     sort(lbvec.begin(), lbvec.end(), [](int a, int b) -> bool { return a < b; });   // Lambda表达式
//     cout << "lambda expression:" << endl;
//     for (int it : lbvec)
//         cout << it << ' ';
//     cout<<endl;
//     return 0;
// }
//lambda表达式就是一个函数，函数就得有输入和输出，所以lambda就得有输入的数据，返回的数据类型，已经函数体
int main()
{
    int a = 6;
    int b = 2;
    // auto add = [a,b](int c)->int {return a*b*c;};
    // cout<<add(4)<<endl;
    int x = 8, y = 9;
    auto add = [](int a, int b) { return a + b; };
    std::function<int(int, int)> Add = [=](int a, int b) { return a + b; };

    cout << "add: " << add(x, y) << endl;
    cout << "Add: " << Add(x, y) << endl;
}
