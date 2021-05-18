#include <iostream>


class parent{
    public:
        int a;
        parent(int _a):a(_a){}
        void display()
        {
            std::cout<<this->a<<std::endl;
        }
};

class child:public parent{
    public:
        child(int _a):parent(_a),a(_a){}
        int a;
};

int main()
{
    
}