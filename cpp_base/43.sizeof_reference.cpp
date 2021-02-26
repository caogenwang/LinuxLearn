#include <iostream>
using namespace std;

int main()
{
    int a = 10;
    int &b = a;

    double c = 10.0L;
    double &d = c;
    cout<<"the size of rreference"<<endl;
    cout<<"size:"<<sizeof(b)<<endl;
    cout<<"size:"<<sizeof(d)<<endl;

    int *p_a = &a;
    double *p_c = &c;

    cout<<"the size of pointer"<<endl;
    cout<<"size:"<<sizeof(p_a)<<endl;
    cout<<"size:"<<sizeof(p_c)<<endl;
    
    return 0;
}
/*结论：sizeof 引用的时候，大小是对应变量的大小
       sizeof 指针的时候，就是指针的大小，为一个int的大小
*/