#include<string>  
#include<iostream>  
using namespace std;

template<typename T>
class Shared_ptr{
public:
    Shared_ptr():count(0),_ptr((T*)0){}
    Shared_ptr(T*p):count(new int(1)),ptr(p){}
    Shared_ptr(Shared_ptr<T> &other):count(&(++ *other.count)),_ptr(other._ptr){}
    // 重载 operator*和operator-> 实现指针功能
    T*operator->(){return _ptr;}
    T&operator*(){return *_ptr;}

    Shared_ptr<T>& operator=(Shared_ptr<T>& other)
    {
        if(this == &other)
            return *this;
        ++ *other.count;
        if(this->_ptr && 0 == --*this->count)
        {
            delete count;
            delete _ptr;
            cout<<"delete ptr ="<<endl;
        }
        this->_ptr = other._ptr;
        this->count = other.count;
        return *this;
    }
    // 析构函数 使引用次数减一并判断引用是否为零(是否调用delete)。
	~Shared_ptr()
	{
		if (_ptr && --*count == 0)
		{
			delete count;
			delete _ptr;
			cout << "delete ptr ~" << endl;
		}
	}
    int getRef(){return *count;}
private:
    int *count;
    T *_ptr;
}

