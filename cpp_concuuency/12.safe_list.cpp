//线程安全的链表，就是在多线程的条件下，不用对全局的链表加锁，操作链表的每个链表的节点都不用加锁
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <unistd.h>
using namespace std;
template<typename T>
class threadsafe_list{
    struct node{
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node():next(){}
        node(T const& value):data(std::make_shared<T>(value)){}
    };
    node head;//它属于链表的头
public:
    threadsafe_list(){}
    ~threadsafe_list(){
        remove_if([](T const&){return true;});
    }
    threadsafe_list(threadsafe_list const &other) = delete;
    threadsafe_list& operator=(threadsafe_list const& other)=delete;

    void push_front(T const& value)
    {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(head.m);
        cout<<value<<endl;
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }

    template<typename Function>
    void for_each(Function f)
    {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);//head节点上锁
        while (node*const next=current->next.get())//头部下一个节点
        {
            std::unique_lock<std::mutex> next_lk(next->m);//锁住当前节点后，立即释放上一个节点
            lk.unlock();//释放上一个节点，其他线程可以进行操作
            f(*next->data);//处理当前节点
            current = next;
            lk=std::move(next_lk);////向后移动，unique_lock is moveable not copyable，而lock_guard不具备移动语义，可见unique_lock比lock_guard灵活
        }
    }
    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p)//找到链表中事谓词P返回true的第一个元素
    {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next=current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();//拿到当前元素的锁后立即释放上一个锁
            if(p(*next->data))//谓词P返回true，那么返回该元素
            {
                return next->data;
            }
            current=next;
            lk=std::move(next_lk);
        }
        return std::shared_ptr<T>();//没有找到返回一个空值
    }

    template<typename Predicate>
    void remove_if(Predicate p)//删除哪些使得谓词P返回true的元素
    {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next=current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            if(p(*next->data))
            {
                std::unique_ptr<node> old_next=std::move(current->next);
                current->next=std::move(next->next);//重置连接
                next_lk.unlock();//注意这里并没有对lk解锁或者重置
            }
            else
            {
                lk.unlock();
                current=next;
                lk=std::move(next_lk);
            }
        }
    }   
};

threadsafe_list<int> list;
void add(int value)
{
    list.push_front(value);
}
int main()
{   
    vector<thread> ths;
    for (int i = 0; i < 1000; i++)
    {
        ths.push_back(thread(add,i));
    }
    for (int i = 0; i < 1000; i++)
    {
        if (ths[i].joinable())
        {
            ths[i].join();
        }
    }
    return 0;
}
