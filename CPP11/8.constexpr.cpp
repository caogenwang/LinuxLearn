#include<iostream>
using namespace std;

int main(){
    int a = 20 ;
    constexpr int x =  a;//error,必须是常量表达式
    return 0;
}