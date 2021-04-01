#include <vector>
#include <iostream>
using namespace std;

class myFuntor{
    public:
        myFuntor(int _d):d(_d){}
        bool operator()(int x){
            return x > d;
        }
    private:
    int d;
};

int main()
{
    vector<int> list{5,7,90,45,78,34,12};
    bind2nd(greater<int>(),4);
    cout << "count: " << std::count_if(list.begin(), list.end(), myFuntor(34));
}