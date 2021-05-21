#include <iostream>
using namespace std;


class Base1{
    public:
        Base1(){
            cout<<"Base1"<<endl;
        };
        virtual ~Base1(){
             cout<<"~Base1"<<endl;
        }
        virtual void speakClearly(){
            cout<<"speakclearly"<<endl;
        }
        virtual Base1 *clone() const{
            Base1 *pb =  new Base1();
            return pb;
        }
    protected:
        float data_base1;
};

class Base2{
    public:
        Base2(){
            cout<<"Base2"<<endl;
        };
        virtual ~Base2(){
             cout<<"~Base2"<<endl;
        }
        virtual void mumble(){
            cout<<"mumble"<<endl;
        }
        virtual Base2 *clone() const{
            Base2 *pb =  new Base2();
            return pb;
        }
    protected:
        float data_base2;
};

class Dev:public Base1,public Base2{
    public:
        Dev(){
            cout<<"Dev"<<endl;
        };
        virtual ~Dev(){
             cout<<"~Dev"<<endl;
        }
        virtual Dev *clone() const{
            Dev *pd =  new Dev();
            return pd;
        }
    protected:
        float data_Dev;
};

int main()
{
    // Base1* pbase2 = new Dev();
    // delete pbase2;

    
    // Dev* pder = new Dev;
    // //调用Base2::mumble()
    // //pder必须向前调整sizeof（Base1）个bytes
    // // pder->mumble();
    // pder->clone();
    // delete pder;

    
    Base2* pbase2_1 = new Dev;
    //调用Derived* Derived::clone()
    //返回值必须被调整，以指向Base2子对象
    Base2* pbase2_2 = pbase2_1->clone();

    delete pbase2_1;
    delete pbase2_2;
}