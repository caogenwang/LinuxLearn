#include<iostream>

using namespace std;

class A{
    public:
        virtual void f(){
            cout<<"hello"<<endl;
        }
};

class B:public A
{
    public:
    virtual void f()
    {
        cout<<"hello2"<<endl;
    }
  
};

class C
{
  void pp()
  {
      return;
  }
};

int fun()
{
    return 1;
}

int main()
{
    A* a1=new B;//a1是A类型的指针指向一个B类型的对象
    A* a2=new A;//a2是A类型的指针指向一个A类型的对象
    B* b;
    C* c;
    b=dynamic_cast<B*>(a1);//结果为not null，向下转换成功，a1之前指向的就是B类型的对象，所以可以转换成B类型的指针。
    if(b==NULL)
    {
        cout<<"null"<<endl;
    }
    else
    {
        cout<<"not null"<<endl;
    }
 
    b=dynamic_cast<B*>(a2);//结果为null，向下转换失败
    // b->f();//出错了，但是static_cast可以成功
    if(b==NULL)
    {
        cout<<"null"<<endl;
    }
 
    else
    {
        cout<<"not null"<<endl;
    }
    b = static_cast<B*>(a2);
    b->f();//static_cast可以成功,hello,基类的函数
    if(b==NULL)
    {
        cout<<"null"<<endl;
    }
    else
    {
        cout<<"not null"<<endl;
    }
 
    c=dynamic_cast<C*>(a1);//结果为null，向下转换失败
    if(c==NULL)
    {
        cout<<"null"<<endl;
    }
 
    else
    {
        cout<<"not null"<<endl;
    }
 
    delete(a1);
    return 0;
}



