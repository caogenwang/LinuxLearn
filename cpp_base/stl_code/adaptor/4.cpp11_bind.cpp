/*
bind函数

    在c++11之前,要绑定某个函数、函数对象或者成员函数的不同参数值需要用到不同的转换器,
    如bind1st、bind2nd、fun_ptr、mem_fun和mem_fun_ref等.

    在c++11中,绑定参数的方法得以简化.c++11提供了"一站式"绑定模板bind,其用法为:

    #include <functional>
    std::bind(待绑定的函数对象/函数指针/成员函数指针,参数绑定值1,参数绑定值2,...,参数绑定值n);
    bind的第一个参数是待绑定的函数对象或者函数指针,之后跟随多个参数以设定待绑定函数的参数绑定方式。
    待绑定函数有多少个参数,则bind后便需要多少个参数以一一声明其参数的绑定方法.当参数绑定为某一固定值时,
    则其对应参数绑定值可以使一个变量或常量.当需要将参数与绑定所生成的函数对象的某个参数相关联时,
    则需要用到在标准中预定义的几个常量_1、_2、_3等.这些常量声明在std::placeholders命名空间内.

    绑定成员函数的注意点:

    在将一个R (T::*ptr)(Arg0,Arg1,...)形式的成员函数指针ptr用bind绑定参数时,
    bind的第一个绑定的参数是成员函数的调用者,随后跟随成员函数的参数绑定方式.
    例如bind(ptr,a,b,c)将会调用a.*ptr(b,c)。当采用_n常量将首参数与函数对象的参数相关联时,
    所生成的函数对象自动可接受T类型的引用及指针类型,无需再进行封装.但要想调用外部数据的成员函数,
    还需要用ref()、cref()来包装或者绑定一个对该变量的指针.
*/

#include <functional>
#include <iostream>
using namespace std;
//一个自定义的整数类
struct integer{
      int i;
      integer(int i):i(i){}
      void incr_by(int j){ i+=j;}
};

void mem_func_sample(){
     integer x(0);    //x.i=0;
     integer *y=&x;   //y指向x
     using namespace std::placeholders;
     using namespace std;

     auto f0=bind(&integer::incr_by,_1,_2);
     f0(x,2);     //x.incr_by(2)
     cout<<x.i<<endl;

     f0(y,2);         //y->incr_by(2)
     cout<<x.i<<endl;   //x.i=4

     auto f1=bind(&integer::incr_by,x,_1);
     f1(2);   //x在f1的副本.incr(2)
     cout<<x.i<<endl;  x.i=4;

     auto f2=bind(&integer::incr_by,ref(x),_1);
     f2(2);   //x.incr_by(2)
     cout<<x.i<<endl;       //x.i=6;

     auto f3=bind(&integer::incr_by,&x,_1);
     f3(2);   //(&x)->incr_by(2)
     cout<<x.i<<endl;  //x.i=8

     //利用mem_fn将成员函数转换为函数对象
     auto f4=mem_fn(&integer::incr_by);
     f4(x,2);
     cout<<x.i<<endl;//x.i=10;
     f4(y,2);
     cout<<x.i<<endl;//x.i=12

}

int main(){
    mem_func_sample();
    return 0;
}
