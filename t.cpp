#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <random>
using namespace std;

thread_local default_random_engine errorCode;

void *MaySetErr(void *input){
    if (*(int*)input == 1)
    {
        errorCode = 1;
    }
    else if(*(int*)input == 2)
    {
        errorCode = -1;
    }
    else
    {
        errorCode = 0;
    }   
}

int main()
{
    int input_a = 1;
    int input_b = 2;

    pthread_t t1,t2;
    pthread_create(&t1,nullptr,&MaySetErr,&input_a);
    pthread_create(&t2,nullptr,&MaySetErr,&input_b);

    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);
    cout<<errorCode<<endl;
    return 0;
}