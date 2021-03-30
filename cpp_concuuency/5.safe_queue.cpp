#include "safe_queue.h"

template <typename T>
void ThreadSafeQueue::Insert(T value){ 
    std::lock_guard<std::mutex> lk(mut_);
    que_.push_back(value);
    cond_.notify_one();
}

template <typename T>
void ThreadSafeQueue::Popup(T &value){
    std::unique_lock<std::mutex> lk(mut_); 
    cond_.wait(lk, [this]{
        return !que_.empety();});//如果空了就等待
    value = que_.front();     
    que_.pop();
}

template <typename T>
bool ThreadSafeQueue::Empty() const
{
    std::lock_guard<std::mutex> lk(mut_);
    return que_.empty();
}