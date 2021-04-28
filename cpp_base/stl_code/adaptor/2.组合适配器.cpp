/*
对于一般的逻辑运算，有时可能还需要对结果求一次逻辑反。
unarynegate和binarynegate实现了这一适配功能。STL还提供了not1和not2辅助生成相应的函数对象实例，分别用于一元谓词和二元谓词的逻辑取反。
例：ptr_fun、not1和not2产生函数适配器实例
*/

#include <functional>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

bool g(int x, int y) {
    return x > y;
}

int main(){
    vector<int> a{30, 90, 10, 40, 70, 50, 20, 80};
    vector<int>::iterator p;
    p = find_if(a.begin(), a.end(), bind2nd(ptr_fun(g), 40));
    /*
        ptr_fun:将一般函数指针转换为函数对象，使之能够作为其它函数适配器的输入
        在进行参数绑定或其他转换的时候，通常需要函数对象的类型信息，
        例如bind1st和bind2nd要求函数对象必须继承于binary_function类型。
        但如果传入的是函数指针形式的函数对象，则无法获得函数对象的类型信息
    */
    p = find_if(a.begin(), a.end(), not1(bind2nd(greater<int>(), 15)));
    
    p = find_if(a.begin(), a.end(), bind2nd(not2(greater<int>()), 15));

}