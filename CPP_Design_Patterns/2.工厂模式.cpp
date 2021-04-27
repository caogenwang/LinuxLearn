#include <iostream>
using namespace std;

class Car{
    public:
        virtual void run() = 0;
        virtual ~Car(){}
};

/*具体的Car*/
class BenzCar:public Car{
    public:
    virtual void run(){
        cout<<"This is a BenzCar"<<endl;
    }
};

class AudiCar:public Car{
    public:
    virtual void run(){
        cout<<"This is a AudiCar"<<endl;
    }
};

class BWMCar:public Car{
    public:
    virtual void run(){
        cout<<"This is a BWMCar"<<endl;
    }
};

class CarFactory{
    public:
    virtual Car *CreateCar() = 0;
    virtual ~CarFactory(){}

};
/*具体的Car factory*/
class BenzCarFactory:public CarFactory{
    public:
        Car *CreateCar(){
            return new BenzCar();
        }
};

class AudiCarFactory:public CarFactory{
    public:
        Car *CreateCar(){
            return new AudiCar();
        }
};

class BWMCarFactory:public CarFactory{
    public:
        Car *CreateCar(){
            return new BWMCar();
        }
};

class MainForm{

    CarFactory *factory;
    public:
    MainForm(CarFactory *factory){
        this->factory = factory;
    }

    Car *getACar()
    {
        return this->factory->CreateCar();
    }
};

int main()
{
    // BenzCarFactory Factory;
    // AudiCarFactory Factory;
    BWMCarFactory Factory;
    MainForm *mainform = new MainForm(&Factory);//使用什么类型的工厂产生什么car
    Car *car = mainform->getACar();
    car->run();
    delete mainform;
    delete car;
}

/*

1. 工厂模式，就是为了解耦和扩展，使工厂和对象都是抽象的，不依赖于具体的工厂对象和对象的类型，利用的是抽象基类的虚函数
    在运行时进行绑定。主要是因对需求经常变化的情况。

2. 工厂设计为抽象基类，工厂的产品也是抽象基类；
    具体的工厂和具体的产品都去继承抽象基类，分别完成自己的功能；
    使用者只要完成抽象基类的主程序流程，在生成具体对象时，会根据
    用户选用的具体工厂类型，动态的去绑定，然后具体的工厂生产具体的产品。
    扩展的时候不用修改之前的代码，是增量修改

3.  应用场景有哪些？
    文档转换成不同格式需要不同的对象方法：
    抽象方法工厂：返回具体的一个方法的对象
    抽象方法定义一些公有的方法：具体类继承抽象类，同时继承里边的方法

    先得到一个具体类型的方法工厂类对象，调用其中功能方法就可以了
4. 与策略不同点：
    一个侧重获取不同的方法
    一个侧重获取不同的对象
*/