
# include <new>   
# include <iostream>   
int main()   
{  using namespace std;   
    char * p = new(nothrow) char [4];   
    if (p == NULL)   
    {  
        cout <<"allocte failed" <<endl;  
        exit(-1);    
    }   
    // ... 

    long * q = new(p)long(1000);   
    delete []p;    //只释放 p，不要用q释放。
    return 0;
} 
/*
    placement new 的主要用途就是可以反复使用一块已申请成功的内存空间。
    这样可以避免申请失败的徒劳，又可以避免使用后的释放。 
    特别要注意的是对于 placement new 绝不可以调用的delete, 因为该new只是使用别人替它申请的地方
    （只是个租房户，不是房主。无权将房子卖掉）。释放内存是nothrow new的事，即要使用原来的指针释放内存
*/