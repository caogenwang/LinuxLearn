#include <iostream>
#include <functional>
#include <set>
using namespace std;

int main()
{
    greater<int> ig;
    cout<<boolalpha<<ig(4,6)<<endl;;
    cout<<greater<int>()(6,4)<<endl;
}