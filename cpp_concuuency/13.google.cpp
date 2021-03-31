#include <thread>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
using namespace std;
std::mutex m;
std::condition_variable con[4];
int g_value = 0;
std::condition_variable g_con;
void worker1(int i){
    while (1)
    {
        sleep(1);
        unique_lock<mutex> ulk(m);
        while (g_value%4 != i)
            con[0].wait(ulk);
        cout<<"worker:"<<i<<" "<<g_value<<endl;
        g_value = g_value + 1;
        con[1].notify_one();
    }   
}

void worker2(int i){
    while (1)
    {
        unique_lock<mutex> ulk(m);
        while (g_value%4 != i)
            con[1].wait(ulk);
        cout<<"worker:"<<i<<" "<<g_value<<endl;
        g_value = g_value + 1;
        con[2].notify_one();
    }   
}

void worker3(int i){
    while (1)
    {
        unique_lock<mutex> ulk(m);
        while (g_value%4 != i)
            con[2].wait(ulk);
        cout<<"worker:"<<i<<" "<<g_value<<endl;
        g_value = g_value + 1;
        con[3].notify_one();
    }  
}

void worker4(int i){
     while (1)
    {
        unique_lock<mutex> ulk(m);
        while (g_value%4 != i)
            con[3].wait(ulk);
        cout<<"worker:"<<i<<" "<<g_value<<endl;
        g_value = g_value + 1;
        con[0].notify_one();
    }  
}

void worker(int i){
     while (1)
    {
        sleep(1);
        unique_lock<mutex> ulk(m);
        while (g_value%4 != i)
            g_con.wait(ulk);
        cout<<"worker:"<<i<<" "<<g_value<<endl;
        g_value = g_value + 1;
        g_con.notify_one();
    }  
}

int main()
{
    vector<thread> ths;
    for (int i = 0; i < 4; i++)
    {
        ths.push_back(thread(worker,i));
    }
    
    for (int i = 0; i < 4; i++)
    {
        ths[i].join();
    }
    return 0;
}