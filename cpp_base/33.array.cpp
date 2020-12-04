#include <iostream>
#include <array>
#include <tuple>
using namespace std;

/*
    数组的模板类
    array<T,N> 模板定义了一种相当于标准数组的容器类型。它是一个有 N 个 T 类型元素的固定序列。
    除了需要指定元素的类型和个数之外，它和常规数组没有太大的差别。显然，不能增加或删除元素
*/
// int main()
// {
//     std::array<double,10> myarray;

//     cout<<"size:"<<myarray.size()<<endl;
//     cout<<"max_size:"<<myarray.max_size()<<endl;

//     for (int i = 0; i < myarray.size()+10; i++)//这里已经写到数组的外界去了
//     {
//         myarray[i] = i * 10.0;
//     }

//     for (int i = 0; i < myarray.size()+10; i++)//这里已经读到数组的外界去了。值的结果是不确定的
//     {
//         cout<<myarray[i]<<endl;
//     }


//     std::array<double,10> darray{0.1,0.2,0.3};
//     // darray.fill(0.5);
//     darray.swap(myarray);
    
//     for (int i = 0; i < darray.size(); i++)//这里已经读到数组的外界去了。值的结果是不确定的
//     {
//         cout<<darray.at(i)<<endl;
//     }
// }

// int main(void) {
//     std::array<int,3> myarray = {10, 20, 30};
//     std::tuple<int, int, int> mytuple (10, 20, 30);
 
//     std::tuple_element<0, decltype(myarray)>::type myelement;  // int myelement
//     std::cout << "myelement: "<<myelement<<endl;
//     myelement = std::get<2>(myarray);
//     std::get<2>(myarray) = std::get<0>(myarray);
//     std::get<0>(myarray) = myelement;//交换了myarray的两个元素
 
//     std::cout << "first element in myarray: " << std::get<0>(myarray) << std::endl;
//     std::cout << "first element in mytuple: " << std::get<0>(mytuple) << std::endl;
 
//     return 0;
// }


int main()
{

    std::array<double,10> myarray{0.5,0.3,0.2,0.1};
    std::array<double,10> oarray = myarray;

    for (int i = 0; i < oarray.size(); i++)
    {
        cout<<oarray[i]<<endl;
    }
    
}