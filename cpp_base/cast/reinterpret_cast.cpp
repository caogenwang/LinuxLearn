#include <iostream>

using namespace std;
//在C++语言中，reinterpret_cast主要有三种强制转换用途：
// 改变指针或引用的类型、将指针或引用转换为一个足够长度的整形、将整型转换为指针或引用类型

// 用法：reinterpret_cast<type_id> (expression)
// type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。
// 它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
// 在使用reinterpret_cast强制转换过程仅仅只是比特位的拷贝，因此在使用过程中需要特别谨慎！
int main(){
    int *a = new int;
    double *d = reinterpret_cast<double *>(a);
    // 将整型指针通过reinterpret_cast强制转换成了双精度浮点型指针。
    //reinterpret_cast可以将指针或引用转换为一个足够长度的整形，
    // 此中的足够长度具体长度需要多少则取决于操作系统，如果是32位的操作系统，就需要4个字节及以上的整型，如果是64位的操作系统则需要8个字节及以上的整型

}