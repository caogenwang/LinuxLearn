#include <iostream>
using namespace std;

template<typename T>
void printBigger(T num1,T num2)  //普通模板
{
    T bigger=num1>num2?num1:num2;
    cout<<"reg:"<<bigger<<endl;
}
template<>  //因为模板特化，指明指明了函数参数类型，故模板参数类型为空
void printBigger(int* num1,int* num2) //特化模板，模板参数类型T的位置用int*代替
{
    int bigger=*num1>*num2?*num1:*num2;
    cout<<"spaical:"<<bigger<<endl;
}
int main()
{
    //当模板特化之后，调用模板函数时，会优先匹配特性化后的函数模板。
    int a=1,b=2;
    int *p=&a;
    int *q=&b;
    printBigger(a,b);//调用普通模板
    printBigger(p,q);
}