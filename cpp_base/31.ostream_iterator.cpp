#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
    std::vector<int> myvector;
    for (int i = 0; i < 10; i++)
    {
        myvector.push_back(i*10);
    }
    std::ostream_iterator<int> out_if(std::cout,",");
    std::copy(myvector.begin(),myvector.end(),out_if);
    return 0;
}