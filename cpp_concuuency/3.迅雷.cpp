// 题目：编写一个程序，开启3个线程，这3个线程的ID分别为A、B、C，
// 每个线程将自己的ID在屏幕上打印10遍，要求输出结果必须按ABC的顺序显示；如：ABCABC….依次递推。
// 采用C++11实现
#include<iostream>  
#include<thread>  
#include<mutex>  
#include<condition_variable>  
using namespace std;  
mutex m;  
condition_variable cond;  
int LOOP=10;  
int flag=0;  
  
void fun(int id){  
    for(int i=0;i<LOOP;i++){  
        unique_lock<mutex> lk(m);  
        while(id!=flag)//一定要用循环判断，若是if多个阻塞线程唤醒后同时处于临界区  
            cond.wait(lk);  
        cout<<(u_char)('A'+id)<<" ";  
        flag=(flag+1)%3;  
        cond.notify_all();  
    }  
}  
int main(){  
    thread B(fun,1);  
    thread C(fun,2);  
    fun(0);  
    cout<<endl;  
    B.join();  
    C.join();  
    return 0;  
} 