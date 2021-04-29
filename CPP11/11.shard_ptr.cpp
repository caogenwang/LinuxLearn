#include<iostream>
#include <vector>
#include<memory>
using namespace std;

class Widget{
public:
    Widget(){
        cout<<"Widget"<<endl;
    }
    void display(){
        cout<<"display"<<endl;
    }
    ~Widget(){
        cout<<"~Widget"<<endl;
    }
};

void func(shared_ptr<Widget> sp){
    cout<<"count:"<<sp.use_count()<<endl;
}

int funcException() { /*throw 1;*/ return 0; } //假设该函数会抛出异常

void demo(shared_ptr<int> sp, int f){}


int main(){
    //1. 陷阱：用同一裸指针创建多个shared_ptr
    //1.1 错误做法
    auto pw = new Widget;
    std::shared_ptr<Widget> spw1(pw); //强引用计数为1，为pw创建一个控制块
    //std::shared_ptr<Widget> spw2(pw); //强引用计数为1，为pw创建另一个新的控制块，会导致多次析构

    auto sp = new Widget;
    func(shared_ptr<Widget>(sp)); //慎用裸指针，sp将在func结束后被释放
    sp->display();

    //1.2 正确做法
    std::shared_ptr<Widget> spw3(spw1); //ok，pw的强引用计数为2。使用与spw1同一个控制块。
    std::shared_ptr<Widget> spw4(new Widget); //将new的结果直接传递给shared_ptr
    std::shared_ptr<Widget> spw5 = std::make_shared<Widget>(); //强烈推荐的做法!

    //2. 陷阱：在函数实参中创建shared_ptr
    //2.1 shared_ptr与异常安全问题
    //由于参数的计算顺序因编译器和调用约定而异。假定按如下顺序计算
    //A.先前new int，然后funcException();
    //B.假设恰好此时funcException产生异常。
    //C.因异常出现shared_ptr还来不及创建，于是int内存泄露
    demo(shared_ptr<int>(new int(100)), funcException());

    //2.2 正确做法
    auto p1 = std::make_shared<int>(100);
    demo(p1, funcException());

    //3. 陷阱：shared_ptr的循环引用（应避免）（见第22课 weak_ptr）

    //4. 删除器
    auto deleter1 = [](Widget* pw) {cout << "deleter1"<< endl; delete pw; };
    auto deleter2 = [](Widget* pw) {cout << "deleter2"<< endl; delete pw; };

    std::shared_ptr<Widget> pw1(new Widget, deleter1);
    std::shared_ptr<Widget> pw2(new Widget, deleter2);

    std::shared_ptr<Widget> pw3(pw1);
    pw3.reset(new Widget); //deleter恢复为默认的std::default_delete

    vector<std::shared_ptr<Widget>> vecs;
    vecs.emplace_back(pw1);
    vecs.emplace_back(pw2); //pw1和pw2虽然有不同的删除器，但类型相同，可以放入同一容器内。

    //5. 其它
    //5.1 shared_ptr的大小
    cout << sizeof(spw1) << endl;//8
    cout << sizeof(pw1) << endl; //8
    //5.2 shared_ptr管理动态数组（建议用std::array、std::vector取代）
    std::shared_ptr<int> pArray1(new int[10], [](int* p) {delete[] p; }); //使用delete[]
    std::shared_ptr<int> pArray2(new int[10], std::default_delete<int[]>()); //使用default_delete<int[]>()
    //5.3 常见操作
    cout << pw1.use_count() << endl; //2

    if (pw1) //pw1.use_count >= 1 ?
    {
        cout << "pw1.use_count >= 1" << endl;
    }
    else
    {
        cout << "pw1.use_count == 0" << endl;
    }
    //5.4 别名构造
    int* p = new int(10);

    std::shared_ptr<int> a(new int(20));
    std::shared_ptr<int> b(a, p);  // alias constructor: co-owns a, points to p。可用于多继承中
                                   // a 和 b拥用相同的控制块，但两者指向的对象不同。由于两者拥用相同的
                                   //的控制块，可认为a和b所指对象具有相同的拥有者，因此10和20两个堆对象
                                   //拥有相同的生命期
    cout << *a << endl; //20
    cout << *b << endl; //10

    return 0;
}