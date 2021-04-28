#include <iostream>
#include <memory>
#include <functional>
#include "Widget.h"
using namespace std;

enum class InvestmentType {itSock, itBond, itRealEstate};
class Investment//投资
{
public:
    virtual ~Investment() {} //声明为virtual,以便正确释放子类对象
};

class Stock : public Investment {};//股票
class Bond : public Investment {};  //债券
class RealEstate : public Investment {}; //不动产

void makeLogEntry(Investment* pInvmt) {}

//工厂函数，类似于工厂模式
template<typename... Ts>
auto makeInvestment(Ts&&... params) //返回unique_ptr智能指针
{
    //自定义deleter
    auto delInvmt = [](Investment* pInvmt) //父类指针
    {
        makeLogEntry(pInvmt);
        delete pInvmt; //delete父类指针，所有析构函数须声明为virtual
    };

    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);//通过交换的大具体的类型，运行多态

    if (0/*a Stock Object should be created*/) {
        pInv.reset(new Stock(std::forward<Ts>(params)...)); //原始指针无法隐式转为unique_ptr，使用reset重置所有权
    }
    else if (1/*a Bond Object should be created*/)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (2/*a RealEstate should be created*/)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }

    return pInv;
}


int main()
{
    //1. unique_ptr作为工厂函数的返回值。
    std::shared_ptr<Investment> sp =  makeInvestment();  //从std::unique_ptr转换到std::shared_ptr（从独占到共享的
                                                         //转换简单而高效） 

    //2. PImpl手法的测试，类中包含了智能指针，同时指正指针的类型又是不完整类型，会由于先后顺序导致编译失败
    Widget w;  //注意Widget的析构函数必须手动实现。否则，则当w析构时编译器会将默认的析构函数inline
               //到这里来，但由于include widget.h在inline动作之前，此时编译器看到的是非完整类型的
               //Impl类。因此Widget类中的unique_ptr析构时，delete前检查出是个非完整类指针，从而报错。
}