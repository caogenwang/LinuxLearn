#include <thread>
#include <iostream>
#include <atomic>
#include <unistd.h>
using namespace std;

atomic<int> g_flag(0);
atomic<int> finish(0);
std::mutex m;
std::condition_variable cond;
unique_lock<mutex> __lk(m);
void worker1(void)
{
    g_flag.store(1,memory_order_release);
    cout<<"this is thread1"<<endl;
    while (finish.load() == 0){
        printf("this is thread1 exit\n");
        cond.wait(__lk);//如果错过通知，则不再通知
    }
    cout<<"thread1 exit"<<endl;
}

void worker2()
{

    g_flag.store(2,memory_order_release);
    cout<<"this is thread2"<<endl;
    finish.store(1,memory_order_release);
    cond.notify_all();
    cout<<"thread2 exit"<<endl;
}

int main()
{
    thread one(worker1);
    thread two(worker2);
    one.detach();
	two.detach();
    
    while(g_flag.load()==0);//等待flag改变值
    
    printf("main thread exit\n");
    pthread_exit(NULL);//主线程到这里退出
}