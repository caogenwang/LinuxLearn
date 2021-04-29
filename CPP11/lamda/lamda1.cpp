#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    //1. 泛型局部函数
    auto f1 = [](auto x, auto y) {return x + y; };
    cout <<"f1(1, 3) = " << f1(1, 3) << endl;
    cout <<"f1(4.5, 6.3) = " << f1(4.5, 6.3) << endl;
    cout << "f1(\"abc\", \"def\") = " << f1(string{ "abc" }, "def") << endl;

    //2. 泛型回调函数
    auto f2 = [](auto x) {cout << x << ","; };
    vector<int> v1{1, 2, 3};
    vector<string> v2{ "a","b","c" };
    for_each(v1.begin(), v1.end(), f2); cout << endl;
    for_each(v2.begin(), v2.end(), f2); cout << endl;

    //3. 泛型lambda与函数指针的转换
    auto f3 = [](auto x){ return x; };
    using Func = int(*)(int);
    Func pf = f3;
    cout << pf(5) << endl;  //5

    cout << "----------------------------------"<< endl;
    //4. 利用泛型lambda模拟实现tuple。
    //(1)make_tuple接受可变参数包，返回一个tuple对象（lambda）。
    //   如，auto tp = make_tuple(1, '2', "3");即tp = [](auto access){return access(1, '2', "3")};
    //   可见参数包己被展开，并保存在tp中，可随时供访问器使用。
    //(2)该tuple的使用方式是：tuple(access)，向其传入某种功能的访问器（如打印，求长度函数）
    //   这种通过tuple+access的方式可以达到访问tuple中元素的目的。注意，access为可接受可变参数包的
    //   可调用对象。
    auto make_tuple = [](auto ...xs) {
        return [=](auto access) { return access(xs...); };
    };

    //4.1 求tuple的长度
    auto length = [](auto xs) { //length(tp)，传入tuple
        return xs([](auto ...z) { return sizeof...(z); }); //通过tp(access)向其传入计算元素个数的访问器。
    };

    //4.2 fmap的功能: 将一个tuple(tpSrc)通过func函数映射成另一个新的tuple。如通过对元素*2，
    //将tuple(1,2,3,4)映射成tuple(2,4,6,8)
    auto fmap = [=](auto func) {  //捕获make_tuple
        return [=](auto tpSrc) {  //捕获func和make_tuple
            return tpSrc([=](auto... xs) { return make_tuple(func(xs)...); }); //tuple+访问器方式
        };
    };

    auto tp = make_tuple(1, '2', "3");
    std::cout << length(tp) << std::endl; // 3
    int len = tp([](auto...z) { return sizeof...(z); }); //通过向tuple传入访问器的方式来使用tuple。
                                                      //注意访问器必须可接受可变参数包。
    std::cout << len << std::endl;

    auto twice = [](auto i) { return 2 * i; };  //映射函数
    auto print = [](auto i) { std::cout << i << " "; return i; };
    auto tp1 = make_tuple(1, 2, 3, 4);
    auto tp2 = fmap(twice)(tp1); //将tp1通过twice函数映射成tp2
    auto tp3 = fmap(print)(tp2); //将tp2通过print函数映射成tp3，并通过print将tp2元素打印出来。
                                 //make_tuple(func(xs)...) 等价于make_tuple(func(xs1),func(xs2),...)
                                 //因参数按从右向左依次传入，所以最终打印结果为8,6,4,1

    return 0;
}
/*输出结果
f1(1, 3) = 4
f1(4.5, 6.3) = 10.8
f1("abc", "def") = abcdef
1,2,3,
a,b,c,
5
----------------------------------
3
3
8 6 4 2
*/