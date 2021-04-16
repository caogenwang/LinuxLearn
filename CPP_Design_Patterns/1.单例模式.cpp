#include <iostream>
#include <mutex>
using namespace std;

class SigalTon{
public:
    static SigalTon *getInstance(){
        if(instance == nullptr)
        {
            std::unique_lock<mutex> lk(m);
            if (instance == nullptr)
            {
                instance = new SigalTon();
            }
        }
        return instance;
    }
    void destory()
    {
        if (instance != nullptr)
        {
            std::unique_lock<mutex> lk(m);
             if (instance != nullptr)
            {
                delete instance;
            }
        }
    }
    private:
    SigalTon(){}
    virtual ~SigalTon(){}
    static SigalTon* instance;
    mutex m;
};

int main()
{
    SigalTon *instance = SigalTon::getInstance();
    // instance->destory();
    return 0;
}