# include <new>   
# include <iostream>   

int main()   
{  using namespace std;   
    char * p = new(nothrow) char [4];   
    if (p == NULL)   
    {  
        cout <<"allocte failed"<<endl;  
        exit(-1);    
    }   
    // ...   
    long * q = new(p)long(1000);   
    cout<<*q<<endl;
    delete []p;    //只释放 p，不要用q释放。   
    return 0;
}