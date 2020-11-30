#include <iostream>
#include <list>
using namespace std;

int main()
{
    list<int> a;
    cout<<"size1:"<<sizeof(a)<<endl;//24

    list<int>::iterator it;
    cout<<"size2:"<<sizeof(it)<<endl;//8,迭代器只内含了一个指针


    list<char> b;
    cout<<"size1:"<<sizeof(b)<<endl;//24
    b.push_back('a');
    b.push_back('b');
    b.push_back('c');

    list<char>::iterator it2;
    
    cout<<"size2:"<<sizeof(it2)<<endl;//8,迭代器只内含了一个指针
}

//list的结构

/*

struct _List_node_base {
  _List_node_base* _M_next;
  _List_node_base* _M_prev;
};

template <class _Tp>
struct _List_node : public _List_node_base {
  _Tp _M_data;
};

*/

#if 0
struct _List_iterator_base {//list自己定义的迭代器
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef bidirectional_iterator_tag iterator_category;//双向的迭代器

  _List_node_base* _M_node;

  _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
  _List_iterator_base() {}

  void _M_incr() { _M_node = _M_node->_M_next; }
  void _M_decr() { _M_node = _M_node->_M_prev; }

  bool operator==(const _List_iterator_base& __x) const {
    return _M_node == __x._M_node;
  }
  bool operator!=(const _List_iterator_base& __x) const {
    return _M_node != __x._M_node;
  }
}; struct _List_iterator_base {//list自己定义的迭代器
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef bidirectional_iterator_tag iterator_category;//双向的迭代器

  _List_node_base* _M_node;

  _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
  _List_iterator_base() {}

  void _M_incr() { _M_node = _M_node->_M_next; }
  void _M_decr() { _M_node = _M_node->_M_prev; }

  bool operator==(const _List_iterator_base& __x) const {
    return _M_node == __x._M_node;
  }
  bool operator!=(const _List_iterator_base& __x) const {
    return _M_node != __x._M_node;
  }
}; 

#endif