#include <iostream>
using namespace std;

class Color{
public:
    virtual void showColor() = 0;
    virtual ~Color(){}
};

class Bag{
protected:
    Color *color;
public:
    void setColor(Color *c)
    {
        this->color = c;
    }
    void virtual showName() = 0;
    virtual ~Bag(){}
};

class Yellow :public Color{
public:
    virtual void showColor(){
        cout<<"Yellow Color"<<endl;
    }
};

class Red :public Color{
public:
    virtual void showColor(){
        cout<<"Red Color"<<endl;
    }
};

class HandBag:public Bag{
public:
    void virtual showName(){
        color->showColor();
        cout<<"HandBag"<<endl;
    }
};

class Wallet:public Bag{
    public:
    void virtual showName(){
        color->showColor();
        cout<<"Wallet"<<endl;
    }
};

/*包的样式维度，包的颜色维度*/
/*包的样式中包含颜色，那么两个维度的扩展不影响，在抽象层面上相互耦合*/
/*比继承方式要好*/

int main(){

    Color *c = new Yellow();
    Bag *bag = new HandBag();
    bag->setColor(c);
    bag->showName();
    delete c;
    delete bag;
    return 0;
}

/*
注意和装饰模式的不同点是组合的不是父类，而是另外的一个抽象基类，这个抽象基类可以称作桥吗？

当一个类内部具备两种或多种变化维度时，使用桥接模式可以解耦这些变化的维度，使高层代码架构稳定。
    桥接模式通常适用于以下场景。
    当一个类存在两个独立变化的维度，且这两个维度都需要进行扩展时。
    当一个系统不希望使用继承或因为多层次继承导致系统类的个数急剧增加时。
    当一个系统需要在构件的抽象化角色和具体化角色之间增加更多的灵活性时。

    桥接模式的一个常见使用场景就是替换继承。我们知道，继承拥有很多优点，
    比如，抽象、封装、多态等，父类封装共性，子类实现特性。继承可以很好的实现代码复用（封装）的功能，
    但这也是继承的一大缺点。

    因为父类拥有的方法，子类也会继承得到，无论子类需不需要，
    这说明继承具备强侵入性（父类代码侵入子类），同时会导致子类臃肿。因此，
    在设计模式中，有一个原则为优先使用组合/聚合，而不是继承。
*/