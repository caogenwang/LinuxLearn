#include<memory>  
#include<iostream>  
#include <unistd.h>
using namespace std;  
  
struct Node  
{  
    shared_ptr<Node> _pre;  
    shared_ptr<Node> _next;  
  
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
  
    cout << "Node1.use_count:"<<Node1.use_count() << endl;  
    cout << "Node2.use_count:"<< Node2.use_count() << endl;  
}  
/*
说明：shared_ptr的使用使得一块空间有两个对象管理，
即头个结点的_next域和下一个指针共同管理，或者又头一个指针和第二个结点的_ptr域共同管理所以其_pCount=2）
*/
//针对循环引用我们有三种解决方案：
//  《1》当只剩下最后一个引用的时候需要手动打破循环引用释放对象。

// 《2》当parent的生存期超过children的生存期的时候，children改为使用一个普通指针指向parent。

// 《3》使用弱引用的智能指针打破这种循环引用。虽然这三种方法都可行，但方法1和方法2都需要程序员手动控制
int main()  
{  
    FunTest();  
    pause();
    return 0;  
}

/*
强引用： 一个强引用是指当被引用的对象仍活着的话，这个引用也存在
        （也就是说，只要至少有一个强引用，那么这个对象 就不会也不能被释放）。
        boost::share_ptr就是强引用。
弱引用： 相对而言，弱引用当引用的对象活着的时候不一定存在。仅仅是当它自身存在的时的一个引用。

    弱引用并不修改该对象的引用计数，这意味这弱引用它并不对对象的内存进行管理。

    在功能上类似于普通指针，然而一个比较大的区别是，弱引用能检测到所管理的对象是否已经被释放，从而避免访问非法内存 r

    weak_ptr的出现就是为了辅助shared_ptr的工作，弥补shared_ptr的不足，解决shared_ptr造成的循环引用问题，而weak_ptr的这种解决方法也就是弱引用
*/