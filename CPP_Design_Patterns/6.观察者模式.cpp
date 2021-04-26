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
    非常麻烦，不利于扩展。统一的这个接口就是你不变的部分，抽象的，至于领导自己想要观察的部分，领导拿到消息通知后自己去处理。

    基类成为统一非接口
    观察这模式是一种事件通知机制，目标和观察者之间的耦合是通过抽象接口，解除了具体对象
    的耦合。
*/

/*
    1. 定义：
        指多个对象间存在一对多的依赖关系，当一个对象的状态发生改变时，
        所有依赖于它的对象都得到通知并被自动更新。
        这种模式有时又称作发布-订阅模式、模型-视图模式，它是对象行为型模式。
    2. 观察者模式是一种对象行为型模式，其主要优点如下。
        降低了目标与观察者之间的耦合关系，两者之间是抽象耦合关系。符合依赖倒置原则。
        目标与观察者之间建立了一套触发机制。

    3. 它的主要缺点如下。
        目标与观察者之间的依赖关系并没有完全解除，而且有可能出现循环引用。
        当观察者对象很多时，通知的发布会花费很多时间，影响程序的效率
    4. 实现观察者模式时要注意具体目标对象和具体观察者对象之间不能直接调用，
        否则将使两者之间紧密耦合起来，这违反了面向对象的设计原则

    5. 在软件系统中，当系统一方行为依赖另一方行为的变动时，可使用观察者模式松耦合联动双方，使得一方的变动可以通知到感兴趣的另一方对象，从而让另一方对象对此做出响应。

        通过前面的分析与应用实例可知观察者模式适合以下几种情形。
        对象间存在一对多关系，一个对象的状态发生改变会影响其他对象。
        当一个抽象模型有两个方面，其中一个方面依赖于另一方面时，可将这二者封装在独立的对象中以使它们可以各自独立地改变和复用。
        实现类似广播机制的功能，不需要知道具体收听者，只需分发广播，系统中感兴趣的对象会自动接收该广播。
        多层级嵌套使用，形成一种链式触发机制，使得事件具备跨域（跨越两种观察者类型）通知。
*/