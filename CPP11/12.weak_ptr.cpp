#include <iostream>
#include <memory> //for smart pointer
#include <unordered_map> //for unordered_map
#include <set>

using namespace std;

class Widget
{
public:
    Widget(int id):ID(id){}
    
    int ID;
};

//1. 利用weak_ptr来缓存对象
//模拟从数据库中加载，并创建shared_ptr指向widget对象
shared_ptr<Widget> loadWidget(int WidgetID)
{
    return make_shared<Widget>(WidgetID); 
}

//带缓存的工厂函数
std::shared_ptr<const Widget> fastloadWidget(int WidgetID) //返回shared_ptr类型
{
    //缓存：weak_ptr类型
    static std::unordered_map<int, std::weak_ptr<const Widget>> cache;

    auto objPtr = cache[WidgetID].lock(); //objPtr的类型为shared_ptr，指向缓存的对象

    if (!objPtr) { //如果对象不在缓存中. 这里省略了缓存中因失效而不断累积std::weak_ptr的处理。
        objPtr = loadWidget(WidgetID);
        cache[WidgetID] = objPtr;
    }

    return objPtr;
}

//2. 观察者模式
//2.1 观察者
class WeatherObservers //抽象观察者
{
public:
    virtual void updateWeatherInfo(int num) = 0;
};
//机场：具体观察者
class Airport : public WeatherObservers
{
public:
    void updateWeatherInfo(int num) override
    {
        std::cout <<"Airport: " << num << endl;
    }
};
//学校：具体观察者
class School : public WeatherObservers
{
public:
    void updateWeatherInfo(int num) override
    {
        std::cout << "School: " << num << endl;
    }
};

//2.1 主题（气象站）
class WeatherStation
{
    using ObserverPtr = std::weak_ptr<WeatherObservers>; //弱引用

    //set集合中保存观察者的弱引用（以ObserverPtr为关键字，基于ownership排序）
    using ObserverList = std::set<ObserverPtr, std::owner_less<ObserverPtr>>;//对于自动以的数据放在stl中，需要自己重载比较函数，还可以自己添加hash函数

    ObserverList obs; //保存所有观察者

    //std::set<std::weak_ptr<WeatherObservers>, std::owner_less<std::weak_ptr<WeatherObservers>>> obs
public:
    //注册观察者
    void registerObserver(const ObserverPtr oPtr)
    {
        if (obs.find(oPtr) == obs.end()) {
            obs.insert(oPtr);
        }
    }
    //注销观察者
    void unregisterObserver(const ObserverPtr oPtr) //oPtr为weak_ptr类型,利用指针作为统一的接口
    {
        if (obs.find(oPtr) != obs.end())
        {
            obs.erase(oPtr);
        }
    }

    //通知各个观察者
    void notifyObservers(int num)
    {
        std::shared_ptr<WeatherObservers> tempPtr;
        for (auto& ob : obs)
        {
            if ((tempPtr = ob.lock())) {
                tempPtr->updateWeatherInfo(num);
            }
        }
    }
};


int main()
{
    //观察者模式
    WeatherStation station;
    std::shared_ptr<Airport> airport(new Airport());//原型是weak_ptr,修改为shard_ptr
    std::shared_ptr<School>  school(new School());

    station.registerObserver(airport);
    station.registerObserver(school);

    station.notifyObservers(1);

    station.unregisterObserver(school);
    station.notifyObservers(2);

    return 0;
}
/*输出结果
Airport: 1
School: 1
Airport: 2
*/