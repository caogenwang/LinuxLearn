// binder2nd的实例构造通常比较冗长，bind2nd函数用于辅助构造binder2nd，产生它的一个实例。
// binder1st和bind1st，将一个具体值绑定到二元函数的第一个参数。
// 例：函数适配器实例——找到数组中第一个大于40的元素
#include <functional>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    vector<int> a{30, 90, 10, 40, 70, 50, 20, 80};
    vector<int>::iterator p = find_if(a.begin(),a.end(),bind2nd(greater<int>(),40));
    /*这个地方的greater是一个两个参数的比较器，判断第一个是否大于第二，第一个参数是迭代器的值，第二个
    是绑定的值
    */
    if (p == a.end())
        cout << "no element greater than 40" << endl;
    else
        cout << "first element greater than 40 is: " << *p << endl;

    return 0;
}

/*
find_if算法在STL中的原型声明为：
template<class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred);
它的功能是查找数组[first, last)区间中第一个pred(x)为真的元素。

*/

