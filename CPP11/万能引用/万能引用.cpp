#include <iostream>
#include <set>     //for multiset
#include <chrono>  //for std::chrono::system_clock::now()
 
using namespace std;

using timepoint_t = std::chrono::system_clock::time_point;

std::multiset<std::string> names;  //全局数据结构

void log(timepoint_t now,const string &content){}
string nameFromIdx(int idx){return "abc";}

//1.普通函数与完美转发函数构成的重载关系
//1.1 形参为string
void logAndAdd(const std::string& name)
{
    auto now = std::chrono::system_clock::now(); //取得当前时间
    log(now, "logAndAdd");
    names.emplace(name);
    cout << "void logAndAdd(const std::string& name)" << endl;
}
//1.2 形参为int：通过索引查找名字，并记录到names中
void logAndAdd(int idx)
{
    auto now = std::chrono::system_clock::now(); //取得当前时间
    log(now, "logAndAdd");
    names.emplace(nameFromIdx(idx));
    cout << "void logAndAdd(int idx)" << endl;
}

//1.3 形参为万能引用类型（即构成完美转发）
template<typename T>
void logAndAdd(T&& name)
{
    auto now = std::chrono::system_clock::now(); //取得当前时间
    log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
    cout << "void logAndAdd(T&& name)" << endl;
}

//2. 构造函数与完美转发函数构成的重载关系
class Person
{
    std::string name;
public:
    template<typename T>
    explicit Person(T&& n):name(std::forward<T>(n)) //完美转发构造函数
    {
        cout << "explicit Person(T&& n):name(std::forward<T>(n))"<< endl;
    } 

    explicit Person(int idx) : name(std::move(nameFromIdx(idx))){}  //形参为int的构造函数
    
    /*以下两个特殊成员函数是编译器自动生成的，为了便于观察，罗列出来
    Person(const Person& rhs);  //复制构造函数（编译器自动生成）
    Person(Person&& rhs);       //移动构造函数（编译器自动生成）
    */
};

class SpecialPerson : public Person
{
public:
    using Person::Person; //继承构造函数

    /*复制构造函数，调用的是基类的完美转发函数！
    error, 因为rhs的类型为SpecialPerson，当调用Person(rhs)时Person类的模板函数
    会产生一个比默认的构造函数更精确的匹配函数Person(SpecialPerson& n)，但name的构造函数中并没有SpecialPerson的重载版本。*/
    //SpecialPerson(const SpecialPerson& rhs): Person(rhs){}

    /*移动构造函数，调用的是基类的完美转发函数，而非默认的移动构造！
    error, 原因同上*/
    //SpecialPerson(SpecialPerson&& rhs) : Person(std::move(rhs)){}
};

int main()
{
    //1. 普通函数与完美转发构成的重载关系
    //1.1 调用普通函数时：void logAndAdd(const std::string& name)
    std::string petName("Darla");
    logAndAdd(petName);                   //传递左值。由于petName是左值，会被复制到names中且无法避免！（一次构造）
    logAndAdd(std::string("Persephone")); //传递右值。创建string临时对象，由于name本身是左值，会并被复制到names中。（一次构造和一次复制）
    logAndAdd("Patty Dog");               //传递字符串字面量。先创建string临时对象，并被复制到names （一次构造和一次复制）

    //1.2 调用模板函数时：void logAndAdd(T&& name)
    logAndAdd(petName);                   //传递左值。一如此前
    logAndAdd(std::string("Persephone")); //传递右值。创建string临时对象，会并被移动到names中。（一次构造和一次移动）
    logAndAdd("Patty Dog");               //传递字符串字面量。将const char[10]&传递给names，（在multiset中直接构造，仅一次构造！！！）

    logAndAdd(22); //调用void logAndAdd(int idx)

    short nameIdx = 0;
    //logAndAdd(nameIdx); //编译失败，因为形参为short，此时函数模板会产生比logAndAdd(int idx)更精确的匹配函数：void logAndAdd(short idx)
                          //从而，转去调用模板实例化后的void logAndAdd(short idx)函数，当模板中调用names.emplace(std::forward<T>(name))时
                          //会将nameIdx这个short类型的实参传给names中的emplace函数，但其并没有形参为short类型的重载函数，因此报错。（注意，
                          //尽管nameIdx可以通过类型提升转化为int类型，从而匹配logAndAdd(int idx)函数，但根据C++匹配的贪婪性，精确匹配优先于
                          //类型提长后的匹配函数。

    //2. 构造函数与完美转发构成的重载关系
    Person p("Nancy");
    //auto cloneOfP(p); //error，相当于Person cloneOfP(p);本意要调用默认的复制构造函数，但由于p是非const左值，此时模板会生成更精确匹配的
                        //Person(Person& n):name(std::forward<Person&>(n))构造函数。这里会将Person对象传入string的构造函数，因此报错。
    const Person p2("Nancy");
    auto cloneOfP(p2);  //ok，调用默认的复制构造函数Person(const Person& p);虽然此时模板函数也会实例化出一个与复制构造函数签名相同的函数。
                        //但根据C++重载匹配规则，当具有相等的匹配程度时，普通函数优先于模板函数调用。

    return 0;
}

