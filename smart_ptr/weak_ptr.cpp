#include <iostream>
#include <shared_ptr.h>
using namespace std;

class CB;
class CA {
  public:
    CA() {
      std::cout << "CA()" << std::endl;
    }
    ~CA() {
      std::cout << "~CA()" << std::endl;
    }
    void set_ptr(std::shared_ptr<CB>& ptr) {
      m_ptr_b = ptr;
    }
  private:
    std::shared_ptr<CB> m_ptr_b;
};

class CB {
  public:
    CB() {
      std::cout << "CB()" << std::endl;
    }
    ~CB() {
      std::cout << "~CB()" << std::endl;
    }
    void set_ptr(std::shared_ptr<CA>& ptr) {
      m_ptr_a = ptr;
    }
  private:
    // std::shared_ptr<CA> m_ptr_a;
    std::weak_ptr<CA> m_ptr_a;//改为weak引用
};

int main()
{
  std::shared_ptr<CA> ptr_a(new CA());
  std::shared_ptr<CB> ptr_b(new CB());
  ptr_a->set_ptr(ptr_b);
  ptr_b->set_ptr(ptr_a);
  std::cout << ptr_a.use_count() << " " << ptr_b.use_count() << std::endl;

  return 0;
}

/*
weak_ptr的原理
weak_ptr是为了配合shared_ptr而引入的一种智能指针，
它指向一个由shared_ptr管理的对象而不影响所指对象的生命周期，也就是，
将一个weak_ptr绑定到一个shared_ptr不会改变shared_ptr的引用计数。不论是否有weak_ptr指向，一旦最后一个指向对象的shared_ptr被销毁，
对象就会被释放。从这个角度看，weak_ptr更像是shared_ptr的一个助手而不是智能指针

*/

// 初始化方式
// 通过shared_ptr直接初始化，也可以通过隐式转换来构造；
// 允许移动构造，也允许拷贝构造。

#include <iostream>
#include <memory>

class Frame {};

int main2()
{
  std::shared_ptr<Frame> f(new Frame());
  std::weak_ptr<Frame> f1(f);                     // shared_ptr直接构造
  std::weak_ptr<Frame> f2 = f;                    // 隐式转换
  std::weak_ptr<Frame> f3(f1);                    // 拷贝构造函数
  std::weak_ptr<Frame> f4 = f1;                   // 拷贝构造函数
  std::weak_ptr<Frame> f5;
  f5 = f;                                         // 拷贝赋值函数
  f5 = f2;                                        // 拷贝赋值函数
  std::cout << f.use_count() << std::endl;        // 1

  return 0;
}

/*
常用操作
w.user_count()：返回weak_ptr的强引用计数；
w.reset(…)：重置weak_ptr。
*/

/*
如何判断weak_ptr指向对象是否存在？

既然weak_ptr并不改变其所共享的shared_ptr实例的引用计数，
那就可能存在weak_ptr指向的对象被释放掉这种情况。
这时，就不能使用weak_ptr直接访问对象。那么如何判断weak_ptr指向对象是否存在呢？
C++中提供了lock函数来实现该功能。如果对象存在，lock()函数返回一个指向共享对象的shared_ptr(引用计数会增1)，
否则返回一个空shared_ptr。weak_ptr还提供了expired()函数来判断所指对象是否已经被销毁。

由于weak_ptr并没有重载operator ->和operator *操作符，因此不可直接通过weak_ptr使用对象，
同时也没有提供get函数直接获取裸指针。典型的用法是调用其lock函数来获得shared_ptr示例，进而访问原始对象。
*/

/*
使用场景
共享对象的线程安全问题
例如：线程A和线程B访问一个共享的对象，如果线程A正在析构这个对象的时候，线程B又要调用该共享对象的成员方法，
此时可能线程A已经把对象析构完了，线程B再去访问该对象，就会发生不可预期的错误。
*/

#include <iostream>
#include <memory>
#include <thread>

class Test {
  public:
    Test(int id) : m_id(id) {}
    void showID() {
      std::cout << m_id << std::endl;
    }
  private:
    int m_id;
};

void thread1(Test* t) {
  std::this_thread::sleep_for(std::chrono::seconds(2));
  t->showID();                      // 打印结果：0
}

int main()
{
  Test* t = new Test(2);
  std::thread t1(thread1, t);
  delete t;
  t1.join();

  return 0;
}

// 在例子中，由于thread1等待2s，此时，main线程早已经把t对象析构了。打印m_id，
// 自然不能打印出2了。可以通过shared_ptr和weak_ptr来解决共享对象的线程安全问题。

#include <iostream>
#include <memory>
#include <thread>

class Test {
  public:
    Test(int id) : m_id(id) {}
    void showID() {
      std::cout << m_id << std::endl;
    }
  private:
    int m_id;
};

void thread2(std::weak_ptr<Test> t) {
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::shared_ptr<Test> sp = t.lock();//t.lock会增加shared_ptr的一次引用计数
  if(sp)
    sp->showID();                      // 打印结果：2
}

int main()
{
  std::shared_ptr<Test> sp = std::make_shared<Test>(2);
  std::thread t2(thread2, sp);
  t2.join();

  return 0;
}

/*
如果想访问对象的方法，先通过t的lock方法进行提升操作，把weak_ptr提升为shared_ptr强智能指针。提升过程中，
是通过检测它所观察的强智能指针保存的Test对象的引用计数，来判定Test对象是否存活。
ps如果为nullptr，说明Test对象已经析构，不能再访问；如果ps!=nullptr，则可以正常访问Test对象的方法。

如果设置t2为分离线程t2.detach()，让main主线程结束，sp智能指针析构，进而把Test对象析构，
此时showID方法已经不会被调用，因为在thread2方法中，t提升到sp时，lock方法判定Test对象已经析构，提升失败！
*/

/*
观察者模式
观察者模式就是，当观察者观察到某事件发生时，需要通知监听者进行事件处理的一种设计模式。

在多数实现中，观察者通常都在另一个独立的线程中，这就涉及到在多线程环境中，共享对象的线程安全问题
(解决方法就是使用上文的智能指针)。这是因为在找到监听者并让它处理事件时，其实在多线程环境中，
肯定不明确此时监听者对象是否还存活，或是已经在其它线程中被析构了，此时再去通知这样的监听者，肯定是有问题的。

也就是说，当观察者运行在独立的线程中时，在通知监听者处理该事件时，应该先判断监听者对象是否存活，
如果监听者对象已经析构，那么不用通知，并且需要从map表中删除这样的监听者对象。其中的主要代码为：
*/

// 存储监听者注册的感兴趣的事件
#include<unordered_map>
unordered_map<int, list<weak_ptr<Listener>>> listenerMap;

// 观察者观察到事件发生，转发到对该事件感兴趣的监听者
void dispatchMessage(int msgid) {
  auto it = listenerMap.find(msgid);
  if (it != listenerMap.end()) {
    for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
      shared_ptr<Listener> ps = it1->lock();            // 智能指针的提升操作，用来判断监听者对象是否存活
      if (ps != nullptr) {                              // 监听者对象如果存活，才通知处理事件
        ps->handleMessage(msgid);
      } else {
        it1 = it->second.erase(it1);                    // 监听者对象已经析构，从map中删除这样的监听者对象
      }
    }
  }
}

/*
解决循环引用
循环引用，简单来说就是：两个对象互相使用一个shared_ptr成员变量指向对方的会造成循环引用，导致引用计数失效。上文详细讲述了循环引用的错误原因和解决办法。

监视this智能指针
在上文讲述shared_ptr的博文中就有讲述到：enable_shared_from_this中有一个弱指针weak_ptr，这个弱指针能够监视this。在调用shared_from_this这个函数时，
这个函数内部实际上是调用weak_ptr的lock方法。lock()会让shared_ptr指针计数+1，同时返回这个shared_ptr。
*/
