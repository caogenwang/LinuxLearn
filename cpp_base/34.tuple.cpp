#include <iostream>
#include <string>
#include <tuple>

using namespace std;

// int main()
// {
//     std::tuple<int, float, double, long, long long> first;

//     std::string str_second_1("_1");
//     std::string str_second_2("_2");

//     // 指定了元素类型为引用 和 std::string, 下面两种方式都是可以的，只不过第二个参数不同而已
//     std::tuple<std::string, std::string> second_1(str_second_1, std::string("_2"));
//     std::tuple<std::string, std::string> second_2(str_second_1, str_second_2);

//     //3、创建一个元组，元组的元素可以被引用, 这里以 int 为例
//     int i_third = 3;
//     std::tuple<int&> third(std::ref(i_third));


//      int i_fourth_1 = 4;
//      int i_fourth_2 = 44;
//      // 下面的两种方式都可以
//      std::tuple<int, int> forth_1    = std::make_tuple(i_fourth_1, i_fourth_2);
//      auto forth_2                    = std::make_tuple(i_fourth_1, i_fourth_2);

//      std::string str_five_1("five_1");
//     // 输出原址值
//     std::cout << "str_five_1 = " << str_five_1.c_str() << "\n";

//     std::tuple<std::string&, int> five(str_five_1, 5);
//     // 通过元组 对第一个元素的修改，str_five_1的值也会跟着修改，因为元组的第一个元素类型为引用。
//     // 使用get访问元组的第一个元素
//     std::get<0>(five) = "five_2";

//     // 输出的将是： five_2
//     std::cout << "str_five_1 = " << str_five_1.c_str() << "\n";

// }

// int main()
// {
//     std::tuple<char, int, long, std::string> first('A', 2, 3, "4");
//     // 使用std::tuple_size计算元组个数
//     int i_count = std::tuple_size<decltype(first)>::value;
//     std::cout << "元组个数=" << i_count << "\n";


//     std::tuple<int, std::string> third(9, std::string("ABC"));
//     // 得到元组第1个元素的类型，用元组第一个元素的类型声明一个变量
//     std::tuple_element<1, decltype(third)>::type val_1;

//     // 获取元组的第一个元素的值
//     val_1 = std::get<1>(third);
//     std::cout << "val_1 = " << val_1.c_str() << "\n";
// }

/*
获取元素的类型:
    获取元组中某个元素的数据类型，需要用到另外一个类型： std::tuple_element 。 
    语法： std::tuple_element<index, tuple> 。 【index】-元组中元素的索引，
    【tuple】哪一个元组

    

*/

/*
元组，可以看作一个包，类比结构体。 需要访问元组的元素时，2 种方法： A、索引访问，B、std::tie 。

　　元组包含一个或者多个元素，使用std::tie解包： 首先需要定义对应元素的变量，再使用tie。 比如，元素第0个元素的类型时 char, 第1个元素类型时int,  那么， 需要定义 一个 char的变量和int的变量， 用来储存解包元素的结果。 

　　一个例子：
*/
// int main(){
//     std::tuple<char, int, long, std::string> fourth('A', 2, 3, "4");

//     // 定义变量，保存解包结果
//     char tuple_0    = '0';
//     int tuple_1        = 0;
//     long tuple_2    = 0;
//     std::string tuple_3("");

//     // 使用std::tie, 依次传入对应的解包变量
//     std::tie(tuple_0, tuple_1, tuple_2, tuple_3) = fourth;

//     // 输出解包结果
//     std::cout << "tuple_0 = " << tuple_0 << "\n";
//     std::cout << "tuple_1 = " << tuple_1 << "\n";
//     std::cout << "tuple_2 = " << tuple_2 << "\n";
//     std::cout << "tuple_3 = " << tuple_3.c_str() << "\n";
// }


// int main()
// {
//     std::tuple<char, int, long, std::string> fourth('A', 2, 3, "4");

//     // 定义变量，保存解包结果
//     char tuple_0    = '0';
//     int tuple_1        = 0;
//     long tuple_2    = 0;
//     std::string tuple_3("cctv");

//     // 使用占位符
//     std::tie(tuple_0, std::ignore, tuple_2, std::ignore) = fourth;

//     // 输出解包结果
//     std::cout << "tuple_0 = " << tuple_0 << "\n";
//     std::cout << "tuple_1 = " << tuple_1 << "\n";
//     std::cout << "tuple_2 = " << tuple_2 << "\n";
//     std::cout << "tuple_3 = " << tuple_3.c_str() << "\n";
// }

/*拼接*/
// int main()
// {
//     std::tuple<char, int, double> first('A', 1, 2.2f);

//     // 组合到一起, 使用auto， 自动推导
//     auto second = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));
//     // 组合到一起，可以知道每一个元素的数据类型时什么 与 auto推导效果一样
//     std::tuple<char, int, double, char, std::string> third = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));

//     // 输出合并后的元组内容
//     int index = 0;
//     std::cout << index++ << " = " << std::get<0>(second) << "\n";
//     std::cout << index++ << " = " << std::get<1>(second) << "\n";
//     std::cout << index++ << " = " << std::get<2>(second) << "\n";

//     std::cout << index++ << " = " << std::get<3>(second) << "\n";
//     std::cout << index++ << " = " << std::get<4>(second).c_str() << "\n";
// }

/*
    这里将采用的时 递归遍历，需要注意，考虑爆栈的情况。其实，tuple也是基于模板的STL容器。 
    因为其可以容纳多个参数，且每个参数类型可不相同，遍历输出则涉及到参数展开的情况，这里以递归的方式实现遍历， 
    核心代码：
*/
template<typename Tuple, size_t N>
struct tuple_show
{
    static void show(const Tuple &t, std::ostream& os)
    {
        tuple_show<Tuple, N - 1>::show(t, os);
        os << ", " << std::get<N - 1>(t);
    }
};


// 偏特性，可以理解为递归的终止
template<typename Tuple>
struct tuple_show < Tuple, 1>
{
    static void show(const Tuple &t, std::ostream &os)
    {
        os <<  std::get<0>(t);
    }
};

// 自己写个函数，调用上面的递归展开，
template<typename... Args>
std::ostream& operator << (std::ostream &os, const std::tuple<Args...>& t)
{
    os << "[";
    tuple_show<decltype(t), sizeof...(Args)>::show(t, os);
    os << "]";

    return os;
}