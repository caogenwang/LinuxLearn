# [C++实现线程同步的几种方式]

 线程同步是指同一进程中的多个线程互相协调工作从而达到一致性。之所以需要线程同步，是因为多个线程同时对一个数据对象进行修改操作时，可能会对数据造成破坏，下面是多个线程同时修改同一数据造成破坏的例子：

```c++
#include <thread>
#include <iostream>

void Fun_1(unsigned int &counter);
void Fun_2(unsigned int &counter);

int main()
{
    unsigned int counter = 0;
    std::thread thrd_1(Fun_1, counter);
    std::thread thrd_2(Fun_2, counter);
    thrd_1.join();
    thrd_2.join();
    system("pause");
    return 0;
}

void Fun_1(unsigned int &counter)
{
    while (true)
    {
        ++counter;
        if (counter < 1000)
        {
            std::cout << "Function 1 counting " << counter << "...\n";
        }
        else
        {
            break;
        }
    }
}

void Fun_2(unsigned int &counter)
{
    while (true)
    {
        ++counter;
        if (counter < 1000)
        {
            std::cout << "Function 2 counting " << counter << "...\n";
        }
        else
        {
            break;
        }
    }
}
```

