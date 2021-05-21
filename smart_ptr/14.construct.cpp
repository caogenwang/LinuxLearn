#include <iostream>

using namespace std;

class Base{
    public:
    Base(){};
    // Base(int _a,int _b):a(_a),b(_b){}
    int a;
    int b;
};

class Dervice:public Base{
    public:
    Dervice(int _a,int _b,int _c):c(_c){};
    int c;
};
