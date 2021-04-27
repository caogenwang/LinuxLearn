/*
    抽象工厂和工厂方法的区别就是：
    工厂针对的实现方法是没有关联的，抽象工厂的实现的是一组有关联关系的功能；

*/
#include <iostream>
using namespace std;

class Animalfeed{
public:
    virtual void feed() = 0;
    virtual ~Animalfeed(){}
};
class Animalteach{
public:
    virtual void teach() = 0;
    virtual ~Animalteach(){}
};
class Animalplay{
public:
    virtual void play() = 0;
    virtual ~Animalplay(){}
};
class Housefeed:public Animalfeed{
public:
    virtual void feed(){
        cout<<"feed house"<<endl;
    }
};
class Houseteach:public Animalteach{
public:
    virtual void teach(){
        cout<<"teach house"<<endl;
    }
};
class Houseplay:public Animalplay{
public:
    virtual void play(){
        cout<<"play house"<<endl;
    }
};
class FarmFactory{
public:
    virtual Animalfeed *CreateAnimalfeed() = 0;
    virtual Animalteach *CreateAnimalteach() = 0;
    virtual Animalplay *CreateAnimalplay() = 0;
    virtual ~FarmFactory(){}
};

class HouseFarmFactory:public FarmFactory{
public:
    virtual Animalfeed *CreateAnimalfeed(){
        return new Housefeed();
    }
    virtual Animalteach *CreateAnimalteach(){
        return new Houseteach();
    }
    virtual Animalplay *CreateAnimalplay(){
        return new Houseplay();
    }
};

class mainForm{
    FarmFactory *factory;
public:
    mainForm(FarmFactory *f):factory(f){
    }
    void run(){
        Animalfeed *hf = factory->CreateAnimalfeed();
        hf->feed();

        Animalteach *ht = factory->CreateAnimalteach();
        ht->teach();

        Animalplay *hp = factory->CreateAnimalplay();
        hp->play();
    }
    virtual ~mainForm(){
        
    }
};

int main()
{
    FarmFactory * f = new HouseFarmFactory();
    mainForm *m = new mainForm(f);
    m->run();

    delete f;
    delete m;
}


/*
    1. 模式的定义与特点
        抽象工厂（AbstractFactory）模式的定义：是一种为访问类提供一个创建一组相关或相互依赖对象的接口，
        且访问类无须指定所要产品的具体类就能得到同族的不同等级的产品的模式结构。
        抽象工厂模式是工厂方法模式的升级版本，工厂方法模式只生产一个等级的产品，而抽象工厂模式可生产多个等级的产品。
    
    2. 使用抽象工厂模式一般要满足以下条件。
        系统中有多个产品族，每个具体工厂创建同一族但属于不同等级结构的产品。
        系统一次只可能消费其中某一族产品，即同族的产品一起使用。

        抽象工厂模式除了具有工厂方法模式的优点外，其他主要优点如下。
        可以在类的内部对产品族中相关联的多等级产品共同管理，而不必专门引入多个新的类来进行管理。
        当需要产品族时，抽象工厂可以保证客户端始终只使用同一个产品的产品组。
        抽象工厂增强了程序的可扩展性，当增加一个新的产品族时，不需要修改原代码，满足开闭原则。

        其缺点是：当产品族中需要增加一个新的产品时，所有的工厂类都需要进行修改。增加了系统的抽象性和理解难度。

*/