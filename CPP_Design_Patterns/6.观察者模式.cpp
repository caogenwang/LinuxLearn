#include <iostream>
#include <list>
using namespace std;

class Listener{
    public:
        virtual void  doProcess(float value) = 0;
        virtual ~Listener(){}
};

class Zuzhang:public Listener{
    public:
        virtual void  doProcess(float value){
            cout<<"zuzhang observer:"<<value<<endl;
        }
};

class Manager:public Listener{
    public:
        virtual void  doProcess(float value){
            cout<<"manager observer:"<<value<<endl;
        }
};

class Chairman:public Listener{
    public:
        virtual void  doProcess(float value){
            cout<<"chairman observer:"<<value<<endl;
        }
};

class Worker{
    public:
        void startWorking(){
            for (int i = 0; i < 3; i++){
                Onprocess(i);
            }
        }
        void Onprocess(float value){
            for (list<Listener*>::iterator it = listeners.begin(); 
                it != listeners.end(); it++){
                (*it)->doProcess(value);
            }
            
        }
        void add_Listener(Listener *ls){
            listeners.push_back(ls);
        }
        void rem_listener(Listener *ls){
            listeners.remove(ls);
        }
        virtual ~Worker(){
            for (list<Listener*>::iterator it = listeners.begin(); 
                it != listeners.end(); it++){
                    delete (*it);
                }
        }
    private:
    list<Listener*>  listeners;
};


int main()
{
    Worker w;
    Zuzhang *z = new Zuzhang();
    Manager *m = new Manager();
    Chairman *c = new Chairman();

    w.add_Listener(z);
    w.add_Listener(m);
    w.add_Listener(c);

    w.startWorking();
    return 0;
}


/*
    也叫事件的通知模式，我的理解就是，你自己在这边干活，领导在旁边观察者你，当你有什么风吹草动立刻会被通知给
    你的领导，而且你的领导可能不止一个，而且每个领导观察的你的行为的重点不一样，有的关注你的态度，有的关注你本身的
    技能，但是为了达到一个统一的通知模式，你需要定义一个统一通知接口，不然为了对接每个不同的领导，你得开发很多不同的接口，
    非常麻烦，不利于扩展。统一的这个接口就是你不变的部分，至于领导自己想要观察的部分，领导拿到消息通知后自己去处理。

    基类成为统一非接口
*/