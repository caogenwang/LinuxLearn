#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>
using namespace std;

template <typename T>
class ThreadSafeQueue{
public:       
    void Insert(T value);    
    void Popup(T &value);
    bool Empety();
private: 
    mutable std::mutex mut_;    
    std::queue<T> que_;
    std::condition_variable cond_;
};