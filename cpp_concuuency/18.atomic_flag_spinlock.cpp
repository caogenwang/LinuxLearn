#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace  std;

int number = 0;
class spinlock_mutex
{
    std::atomic_flag flag;
public:
    spinlock_mutex():flag(ATOMIC_FLAG_INIT)
    {}

    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }

    bool try_lock()
    {
        if (flag.test_and_set(std::memory_order_acquire))
            return false;

        return true;
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};
spinlock_mutex sm;
void pp()
{
    sm.lock();
    for (int i = 0; i < 10000; i++)
    {
        number++;
    }
    sm.unlock();
}

int main()
{
    vector<thread> ths;
    for (int i = 0; i < 5; i++)
    {
        ths.push_back(thread(pp));
    }
    for (int i = 0; i < 5; i++)
    {
        ths[i].join();
    }
    cout<<number<<endl;
    return 0;
}