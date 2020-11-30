#include <iostream>

using namespace std;
class Test
{
public:
    int m_i;
    int m_j;

    struct Obj  // 相当于在类中定义了一个结构体
    {
        struct Obj* pNext;   // 这个pNext是一个嵌入式指针
        // 
    };
};

int main()
{
    Test myTest;
    cout << sizeof(myTest) << endl; // 8字节，结构体只是在类中定义，并不是类中有结构体的成员变量
    Test::Obj* pTemp;    // 定义一个指针
    pTemp = (Test::Obj*)&myTest;   // pTemp 指向对象myTest 的首地址
    pTemp->pNext = nullptr;
    // 因为Test::Obj 结构体中只有一个指针，pTemp 指向的是Test::Obj 里面内容的首地址
    // 而其中的内容只有pNext，所以此时pTemp 和 pTemp->pNext 所指向的是同一块位置
    // pTemp->pNext = nullptr;相当于把myTest 的前4个字节初始化为 00 00 00 00
    // 如果将pTemp->pNext 指向下一块待分配的内存，就实现了空间内存块的链接
}