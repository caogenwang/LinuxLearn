#include <thread>
#include <iostream>
using namespace std;

class Test{
    public:
        void operator()()const{
            cout<<"operator()"<<*data<<endl;
        }
        Test(int a = 10){
            cout<<"constructor"<<endl;
            data = new int(a);
        }
        int get()const {
            return *data;
        }
        Test(const Test& one){
            cout<<"copy constructor"<<endl;
        }
    private:
    int *data;
};

int main()
{
    Test one;
    thread myThread(one);//函数对象被拷贝进线程空间
    myThread.join();
    return 0;
}