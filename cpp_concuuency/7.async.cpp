#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
using namespace std;
int find_the_answer_to_ituae()
{
    return 10;
}
void do_other_stuff(){
    cout<<"I am sleep 5 second"<<endl;
    sleep(5);
}
int main ()
{
   std::future<int> the_answer = std::async(find_the_answer_to_ituae);
   do_other_stuff();
   std::cout<<"The answer is "<<the_answer.get()<<std::endl;
}

