#include <iostream>
#include <functional>
#include <vector>
using namespace std;

template<typename T>
class great:public binary_function<T,T,bool>{
public:
    bool operator()(const T& __x, const T& __y){
        return __x < __y;
    }
};

int main()
{
    vector<int> test{1,6,9,3,5,10};
    great<int> g;
    sort(test.begin(),test.end(),great<int>());//临时对象
    for (int i = 0; i < test.size(); i++)
    {
        cout<<test[i]<<endl;
    }
    
}
