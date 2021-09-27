#include <iostream>
using namespace std;

const int &Search(const int *a,int n,int val);

int main(){
    int a[10] = {0,1,2,3,4,5,6,7,8,9};
    int val = 5;
    int &p = const_cast<int &>(Search(a, 10, val));

    if(p == NULL)
        cout<<"Not found the val in array a"<<endl;
    else
        cout<<"hvae found the val in array a and the val = "<<p<<endl;
    return 0;
}

const int & Search(const int * a, int n, int val)
{
    int i;
    for(i=0; i<n; i++)
    {
        if(a[i] == val)
        return a[i];//注意，引用和指针是没有关系的
    }
    return NULL;
}

// 了解了const_cast的使用场景后，可以知道使用const_cast通常是一种无奈之举，
// 同时也建议大家在今后的C++程序设计过程中一定不要利用const_cast去掉指针或引用的常量性并
// 且去修改原始变量的数值，这是一种非常不好的行为。







