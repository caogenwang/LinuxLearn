#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
using namespace std;

void display(vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout<<vec[i]<<endl;
    }
    cout<<"-------------"<<endl;
}

int main()
{
    int arr[10]={1,2,3,4,5,6,7,8,9,0};
    // vector<int> vec(arr,arr+7);
    vector<int> vec;
    vector<int> vec1;
    // vec.reserve(1);
    // cout<<"size: "<<vec.size()<<endl;
    // cout<<"capacity: "<<vec.capacity()<<endl;
    // display(vec);

    // cout<<"size: "<<vec.size()<<endl;
    // cout<<"capacity: "<<vec.capacity()<<endl;
    // display(vec);


    // vec.resize(5);
    // cout<<"size: "<<vec.size()<<endl;
    // cout<<"capacity: "<<vec.capacity()<<endl;
    // display(vec);

    for(int i=0;i<10;i++)
    {
        vec.push_back(i);
    }
    vec.insert(vec.begin()+2,10);
    display(vec);
    cout<<"size: "<<vec.size()<<endl;
    cout<<"capacity: "<<vec.capacity()<<endl;

    vec.pop_back();
    display(vec);
    cout<<"size: "<<vec.size()<<endl;
    cout<<"capacity: "<<vec.capacity()<<endl;

    // vec.erase(vec.begin()+6);//删除中间，做了copy
    // display(vec);
    // cout<<"size: "<<vec.size()<<endl;
    // cout<<"capacity: "<<vec.capacity()<<endl;

    // vec.clear();
    // cout<<"size: "<<vec.size()<<endl;
    // cout<<"capacity: "<<vec.capacity()<<endl;

    vector<int> dp{1,2,3,4,5,6,7,8,9,0,10,11,12,13};

    dp=vec;
    cout<<"dp-size: "<<dp.size()<<endl;
    cout<<"dp-capacity: "<<dp.capacity()<<endl;
    display(dp);

    cout<<"-------------------------------"<<endl;
    /*resize,capacity,reserve*/
    vector<int> dp2;
    for(int i=0;i<13;i++)
    {
        dp2.push_back(i);
    }
    cout<<"size:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    dp2.resize(15);//补充int(0)
    cout<<"resize:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    for(int i=0;i<dp2.size();i++)
    {
        cout<<dp2[i]<<endl;
    }
    dp2.resize(8);//截取最后的几个元素，capacity
    cout<<"resize:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    for(int i=0;i<dp2.size();i++)
    {
        cout<<dp2[i]<<endl;
    }

    dp2.reserve(12);
    cout<<"size:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    for(int i=0;i<dp2.size();i++)
    {
        cout<<dp2[i]<<endl;
    }

    dp2.reserve(6);
    cout<<"size:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    for(int i=0;i<dp2.size();i++)
    {
        cout<<dp2[i]<<endl;
    }

    dp2.reserve(20);//reserve
    cout<<"size:"<<dp2.size()<<endl;
    cout<<"capacity:"<<dp2.capacity()<<endl;
    for(int i=0;i<dp2.size();i++)
    {
        cout<<dp2[i]<<endl;
    }
    return 0;
}