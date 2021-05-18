#include<iostream>
#include<mutex>
#include<thread>
#include<memory>
#include<vector>
#include<assert.h>
using namespace std;

std::mutex m;
shared_ptr<vector<int> > ptr;
int loop = 100;

void read(){
    while (1)
    {
        {
            shared_ptr<vector<int>> temp_ptr;
            {
                unique_lock<mutex> lk(m);
                temp_ptr = ptr;
                assert(!temp_ptr.unique());
            }
            for(auto it=temp_ptr->begin();it!=temp_ptr->end();it++)//如果存在写者，那么读者访问的是旧的vector
                cout<<*it<<" ";
        }
    }
}

void write(){//写者
    for(int i=0;;i++){
        {//在一个块内使临时对象及时得到析构
            unique_lock<mutex> lk(m);//写者和写者之间，写者和读者之间都要互斥
            if(!ptr.unique())//如果存在其它写者或读者，则需要拷贝当前的vector
                ptr.reset(new vector<int>(*ptr));
            assert(ptr.unique());
            ptr->push_back(i);
        }
    }
}