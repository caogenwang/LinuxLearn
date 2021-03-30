#include<iostream>  
#include<thread>  
#include<mutex>  
#include<condition_variable>  
using namespace std;

int LOOP=10;
int flag=0;
mutex m;  
condition_variable cond;

void display(int id)
{
    int i = id;
    for (int i = 0; i < LOOP; i++)
    {
        unique_lock<mutex> lk(m);
        while (id != flag)
            cond.wait(lk);
        flag=(flag+1)%3;
        cout<<(u_char)('A'+id)<<" "; 
        cond.notify_all();
    }
}

int main()
{
    thread A(display,0);
    thread B(display,1);
    thread C(display,2);

    A.join();
    B.join();
    C.join();
    cout<<endl;
    return 0;
}