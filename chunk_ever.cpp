#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

template<typename T>
vector<vector<T> > chunk_every(vector<T> array,int count,int step=0)
{
   
    int length = ceil(array.size() / count);
    if (step == 0)
    {
        step = length;
    }
    vector<vector<T> > vecs;
    for (int i = 0; i < array.size(); i+=step+1)
    {
        vector<T> tempvec;
        for (int j = i; j - i <= length && j<array.size(); j++)
        {
            tempvec.push_back(array[j]);
        }
        vecs.push_back(tempvec);
    }
    return vecs;
}


int main()
{
    vector<int> vec;
    for (int i = 1; i < 101; i++)
    {
        vec.push_back(i);
    }

    vector<vector<int> > vecs = chunk_every(vec,6);
    cout<<"res:\n";
    for (vector<int> v:vecs){
        for(int i:v)
            cout<<i<<" ";
        cout<<endl;
    }
        
    return 0;
}