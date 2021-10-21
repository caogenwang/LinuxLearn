/*
    策略模式也是一种扩展模式，进行动态时绑定，可以随时扩展新的方法，而不用
    修改之前的代码，实现代码的复用；继承+多态

    本质是一种对方法的扩展
*/

/*
解耦和扩展
不变的是方法，变的是方法的实现
    策略模式的主要优点如下。
        多重条件语句不易维护，而使用策略模式可以避免使用多重条件语句，如 if...else 语句、switch...case 语句。
        策略模式提供了一系列的可供重用的算法族，恰当使用继承可以把算法族的公共代码转移到父类里面，从而避免重复的代码。
        策略模式可以提供相同行为的不同实现，客户可以根据不同时间或空间要求选择不同的。
        策略模式提供了对开闭原则的完美支持，可以在不修改原代码的情况下，灵活增加新算法。
        策略模式把算法的使用放到环境类中，而算法的实现移到具体策略类中，实现了二者的分离。

    其主要缺点如下。
        客户端必须理解所有策略算法的区别，以便适时选择恰当的算法类。
        策略模式造成很多的策略类，增加维护难度。

        1. 模式的结构
            策略模式的主要角色如下。
            1. 抽象策略（Strategy）类：定义了一个公共接口，各种不同的算法以不同的方式实现这个接口，环境角色使用这个接口调用不同的算法，一般使用接口或抽象类实现。
            2. 具体策略（Concrete Strategy）类：实现了抽象策略定义的接口，提供具体的算法实现。
            3. 环境（Context）类：持有一个策略类的引用，最终给客户端调用。
*/


class StrategyFactory{

};

class TaxStrategy{//策略的基类
public:
    virtual double Calculate(const Context& context)=0;
    virtual ~TaxStrategy(){}
};

class Context{//环境持有和设置不同的策略
    public:
    TaxStrategy * taxStrategy;
    void setStrategy( TaxStrategy * taxStrategy)
    {
        this->taxStrategy = taxStrategy;
    }
    void startStrategy(){
        taxStrategy->Calculate(this);//具体的方法
    }
};

class CNTax : public TaxStrategy{
public:
    virtual double Calculate(const Context& context){
        //***********
    }
};

class USTax : public TaxStrategy{
public:
    virtual double Calculate(const Context& context){
        //***********
    }
};

class DETax : public TaxStrategy{
public:
    virtual double Calculate(const Context& context){
        //***********
    }
};



//扩展
//*********************************
class FRTax : public TaxStrategy{
public:
	virtual double Calculate(const Context& context){
		//.........
	}
};


class SalesOrder{
private:
    TaxStrategy* strategy;

public:
    SalesOrder(StrategyFactory* strategyFactory){
        this->strategy = strategyFactory->NewStrategy();
    }
    ~SalesOrder(){
        delete this->strategy;
    }

    public: double CalculateTax(){
        //...
        Context context();
        double val = 
            strategy->Calculate(context); //多态调用
        //...
    }
    
};