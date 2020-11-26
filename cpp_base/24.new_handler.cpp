#include <new>
#include <iostream>
#include <cassert>

using namespace std;

void noMoreMemory()
{
    cerr<<"out of memory"<<endl;
    abort();
}

int main()
{
    set_new_handler(noMoreMemory);
    int *p=new int[100000000000000000];
    assert(p);
}
