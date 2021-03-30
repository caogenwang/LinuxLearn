#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

using namespace std;

int func(int id)
{
    cout<<"child thread:"<<std::this_thread::get_id()<<" "<<id<<endl;
    return id;
}

int main()
{
    std::packaged_task<int(int)> myFunTask(func);
    future<int> myFuture = myFunTask.get_future();
    thread t(std::move(myFunTask),10);
    int id = myFuture.get();
    cout<<"main thread:"<<std::this_thread::get_id()<<" "<<id<<endl;
    return 0;
}