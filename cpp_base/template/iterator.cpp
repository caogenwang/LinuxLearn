#include <iostream>

using namespace std;
//针对链表设计的迭代器
template <typename T>
class ListIter
{
    public:
        typedef  T value_type;
        typedef  T& reference ;
        typedef  T* pointer;
        ListIter(pointer p = NULL) :Iter(p) {}
    //迭代器基本功能，后置++ /前置++/取值*
        ListIter& operator++()//前置自增运算
        {
            Iter = Iter->next();
            return *this;
        }
        ListIter operator++(int)//后置自增运算
        {
            ListIter tmp = *this;
            ++(*this);
            return tmp;
        }
        reference operator*() //解引用，即获取迭代器指向的元素
        {
            return *Iter;
        }
    private:
        pointer Iter;
};

template<typename T>
class ListNode
{
    public:
        ListNode(const T t):Data(t),Next(nullptr){}
        void setnext(ListNode<T>* n)
		{
			Next = n;
		}
		ListNode<T>* next()
		{
			return Next;
		}
		T data(){return Data;}
    private:
		ListNode<T>* Next;
		T Data;
};

//链表
template<typename T>
class list
{
    public:
        typedef typename ListIter<ListNode<T> >::pointer iterator;
        list():Head(NULL) {}
        list(ListNode<T>*pitem):Head(pitem){}
        void push(const T& t)
        {
            ListNode<T>* Data = new ListNode<T>(t);
            Data->setnext(Head);
            Head = Data;
        }
        
        iterator front()
        {
            return Head;
        }
    private:
        ListNode<T>* Head;
};


// int main(int argc,char**argv)
// {
//     list<string> listDemo;
// 	listDemo.push(string("this"));
// 	listDemo.push("is");
// 	listDemo.push("a");
// 	listDemo.push("test");
// 	listDemo.push("example");
// 	ListIter<ListNode<string> > iter(listDemo.front());//获取迭代器,如此设计暴露了
// 	ListIter<ListNode<string> > end;
//     for(int i=0;i<3;i++)
//     {
// 		cout << (*iter).data() <<endl;
// 		iter++;
//     }
//     return 0;
// }
int main(int argc,char**argv)
{
    list<int> lists;
    lists.push(1);
    // lists.push(2);
    list<int>::iterator it = lists.front();
    cout << (*it).data()<<endl;
    it++;
    cout << (*it).data()<<endl;
    // for(int i=0;i<3;i++)
    // {
	// 	cout << (*it).data()<<endl;
	// 	it++;
    // }

    return 0;
}