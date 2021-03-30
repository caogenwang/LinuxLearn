// 是否熟悉POSIX多线程编程技术？如熟悉，编写程序完成如下功能：
// 1）有一int型全局变量g_Flag初始值为0；
// 2） 在主线称中起动线程1，打印“this is thread1”，并将g_Flag设置为1
// 3） 在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
// 4） 线程序1需要在线程2退出后才能退出
// 5） 主线程在检测到g_Flag从1变为2，或者从2变为1的时候退出
#include<iostream>  
#include<functional>  
#include<thread>  
#include<future>  
#include<utility>  
#include<stdio.h>  
#include<chrono>  
#include<atomic>  
#include<condition_variable>
//#include<pthread.h>  
using namespace std;
atomic<int> flag(0);//采用原子操作保护g_Flag的读写  
mutex m;
condition_variable cond1;
condition_variable cond2;
condition_variable cond3;
 
void worker1(int f1){//线程1  
	unique_lock<mutex>lk(m);
	
	printf("this is thread%d\n",f1);
	flag = 1;
	cond3.notify_one();
	while (flag != 2)
		cond1.wait(lk);
	printf("thread1 exit\n");
	cond3.notify_one();
}
 
void worker2(int f2){//线程1  
	unique_lock<mutex>lk(m);
	while (1 != flag)
		cond3.wait(lk);
	printf("this is thread%d\n", f2);
	printf("thread2 exit\n");
	   flag = 2;
	cond1.notify_all();
}
 
 
int main(){
	
	thread one(worker1, 1);
	thread two(worker2, 2);
	 
	one.detach();
	two.detach();
	//pthread_exit(NULL);//主线程到这里退出  
 
	unique_lock<mutex>lk(m);
    printf("main thread waitting\n");
    cond3.wait(lk);
    
	printf("main thread exit\n");
	return 0;
}