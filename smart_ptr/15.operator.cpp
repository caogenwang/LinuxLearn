#include <iostream>

using namespace std;

class Student{
    public:
        Student(){}
        ~Student(){};
        void *operator new(size_t size)
        {
            cout<<"allocator memory"<<endl;
        }
        void operator delete(void*ptr)
        {
            cout<<"deallocator memory"<<endl;
        }
};

int main()
{
    Student *s = new Student();
    delete s;
}