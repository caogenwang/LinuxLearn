#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

std::atomic_flag lock = ATOMIC_FLAG_INIT;
void f(int n)
{
    for (int cnt = 0; cnt < 3; cnt++)
    {
        while (lock.test_and_set(std::memory_order_acquire))//如果被设置了就进行自旋
        {
            //如果被清除了标志位，则退出自旋
        }
        std::cout<<"Output from thread "<<n<<'\n';
        lock.clear(std::memory_order_release);
    }    
}

int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f, n);
    }
    for (auto& t : v) {
        t.join();
    }
}