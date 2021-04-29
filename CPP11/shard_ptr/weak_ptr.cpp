#include<memory>  
#include<iostream> 
#include <unistd.h>
using namespace std;  
  
struct Node  
{  
    weak_ptr<Node> _pre;  
    weak_ptr<Node> _next;  
  
    ~Node()  
    {  
        cout << "~Node():" << this << endl;  
    }  
    int data;  
};  
  
void FunTest()  
{  
    shared_ptr<Node> Node1(new Node);  
    shared_ptr<Node> Node2(new Node);  
    Node1->_next = Node2;  
    Node2->_pre = Node1;  
  
    cout <<"Node1.use_count:"<< Node1.use_count() << endl;  
    cout <<"Node2.use_count:"<< Node2.use_count() << endl;  
}  
  
int main()  
{  
    FunTest();  
    pause(); 
    return 0;  
}
/*
shared_ptr：

       

shared_ptr完成了你所希望的工作:他负责在不使用实例时删除由它指向的对象(pointee)，
并且它可以自由的共享它指向的对象(pointee)。

weak_ptr：

        weak_ptr是为配合shared_ptr而引入的一种智能指针来协助shared_ptr工作，
        它可以从一个shared_ptr或另一个weak_ptr对象构造，它的构造和析构不会引起引用记数的增加或减少。
        没有重载*和->但可以使用lock获得一个可用的shared_ptr对象。

        weak_ptr的一个重要用途是通过lock获得this指针的shared_ptr,使对象自己能够生产shared_ptr来管理自己，
        但助手类enable_shared_from_this的shared_from_this会返回this的shared_ptr，
        只需要让想被shared_ptr管理的类从它继承即可
*/