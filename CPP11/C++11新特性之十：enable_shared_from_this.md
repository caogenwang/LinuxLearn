# C++11新特性之十：enable_shared_from_this

 enable_shared_from_this是一个模板类，定义于头文件<memory>，其原型为：

```c++
template< class T > class enable_shared_from_this;
```

std::enable_shared_from_this 能让一个对象（假设其名为 t ，且已被一个 std::shared_ptr 对象 pt 管理）安全地生成其他额外的 std::shared_ptr 实例（假设名为 pt1, pt2, ... ） ，它们与 pt 共享对象 t 的所有权。
       若一个类 T 继承 std::enable_shared_from_this<T> ，则会为该类 T 提供成员函数： shared_from_this 。 当 T 类型对象 t 被一个为名为 pt 的 std::shared_ptr<T> 类对象管理时，调用 T::shared_from_this 成员函数，将会返回一个新的 std::shared_ptr<T> 对象，它与 pt 共享 t 的所有权。
一.使用场合

       当类A被share_ptr管理，且在类A的成员函数里需要把当前类对象作为参数传给其他函数时，就需要传递一个指向自身的share_ptr。
1.为何不直接传递this指针

       使用智能指针的初衷就是为了方便资源管理，如果在某些地方使用智能指针，某些地方使用原始指针，很容易破坏智能指针的语义，从而产生各种错误。

2.可以直接传递share_ptr<this>么？

       答案是不能，因为这样会造成2个非共享的share_ptr指向同一个对象，未增加引用计数导对象被析构两次。例如：
```c++
#include <memory>
#include <iostream>
 
class Bad
{
public:
	std::shared_ptr<Bad> getptr() {
		return std::shared_ptr<Bad>(this);
	}
	~Bad() { std::cout << "Bad::~Bad() called" << std::endl; }
};
 
int main()
{
	// 错误的示例，每个shared_ptr都认为自己是对象仅有的所有者
	std::shared_ptr<Bad> bp1(new Bad());
	std::shared_ptr<Bad> bp2 = bp1->getptr();
	// 打印bp1和bp2的引用计数
	std::cout << "bp1.use_count() = " << bp1.use_count() << std::endl;
	std::cout << "bp2.use_count() = " << bp2.use_count() << std::endl;
}  // Bad 对象将会被删除两次
```

![img](/Users/yangsen/work/LinuxLearn/CPP11/cpp1.png)

当然，一个对象被删除两次会导致崩溃。

![img](/Users/yangsen/work/LinuxLearn/CPP11/cpp2.png)

正确的实现如下：

```c++
#include <memory>
#include <iostream>
 
struct Good : std::enable_shared_from_this<Good> // 注意：继承
{
public:
	std::shared_ptr<Good> getptr() {
		return shared_from_this();
	}
	~Good() { std::cout << "Good::~Good() called" << std::endl; }
};
 
int main()
{
	// 大括号用于限制作用域，这样智能指针就能在system("pause")之前析构
	{
		std::shared_ptr<Good> gp1(new Good());
		std::shared_ptr<Good> gp2 = gp1->getptr();
		// 打印gp1和gp2的引用计数
		std::cout << "gp1.use_count() = " << gp1.use_count() << std::endl;
		std::cout << "gp2.use_count() = " << gp2.use_count() << std::endl;
	}
	system("pause");
} 
```

![img](/Users/yangsen/work/LinuxLearn/CPP11/cpp3.png)

二.为何会出现这种使用场合

       因为在异步调用中，存在一个保活机制，异步函数执行的时间点我们是无法确定的，然而异步函数可能会使用到异步调用之前就存在的变量。为了保证该变量在异步函数执期间一直有效，我们可以传递一个指向自身的share_ptr给异步函数，这样在异步函数执行期间share_ptr所管理的对象就不会析构，所使用的变量也会一直有效了（保活）。
- 我的疑问是，std::shared_ptr<Good> gp2 = gp1->getptr(); 为什么需要通过这个getptr()去获取呢，直接std::shared_ptr<Good> gp2 = gp1不就行了吗？

  - [subjectxbj](https://blog.csdn.net/subjectxbj)回复**:**这个一般是用在对象的成员函数里面使用的时候。比方说你在成员函数里设置了一个回调函数，就需要传递这个对象指针到回调函数，这样回调函数被调用时，可以通过这个对象指针获取对象的上下文信息。如果是直接传this,那么有可能回调函数被调用的时候，这个对象已经被析构了，这个时候就有问题了。所以需要传一个shared_ptr对象，让reference加1.这样对象就不会销毁。 那么这种情况下，你就没法直接获取一个share_ptr对象，所以就用到了enable_shared_from_this1 年前回复![img](https://csdnimg.cn/release/blogv2/dist/pc/img/commentMore.png)

    

    [syueXj](https://blog.csdn.net/xujia1126608912)回复**:** 我所了解的一种使用场景:某个对象O的成员函数A需要将另一个成员函数B封装成函数对象C供外部使用.假如使用std::bind封装,那么bind的第二参数可以是原生指针,也可是智能指针.假设使用原生指针,那么如果对象的析构早于C的析构,别的代码调用C会产生错误,C再次析构也会产生错误.使用智能指针是一种好的方法,我们用shared_ptr X管理对象O,问题在于如何在函数A中获得共享X的另一个shared_ptr,通过原生指针构建不能共享,而此时我们也没有X对象,继承enable_shared_from_this是一种方法2 年前回复

    

  - [senjoy)[注定会](https://blog.csdn.net/csenjoy)回复**:** 智能指针的用法：直接原生指针构造智能指针，可能会出现释放多次内存的情况。如博主举的例子。 std::shared_ptr<Good> gp2 = gp1->getptr();这个用法是为了演示了 enable_from_this(),可以将原生指针构造出共享的智能指针避免内存重复释放。而std::shared_ptr<Good> gp2 = gp1这样的用法本来就是安全的。没有演示作用



























