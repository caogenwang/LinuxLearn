#include <iostream>
using namespace std;

class Stream{
    public:
    virtual void read() = 0;
    virtual void write() = 0;
    virtual void seek() = 0;
    virtual ~Stream(){}
};

class FileStream:public Stream{
public:
    virtual void read(){
        cout<<"This is FileStream Read"<<endl;
    }

    virtual void write(){
        cout<<"This is FileStream Write"<<endl;
    }

    virtual void seek(){
        cout<<"This is FileStream Seek"<<endl;
    }
};

class BufferStream:public Stream{
public:
    virtual void read(){
        cout<<"This is BufferStream Read"<<endl;
    }

    virtual void write(){
        cout<<"This is BufferStream Write"<<endl;
    }

    virtual void seek(){
        cout<<"This is BufferStream Seek"<<endl;
    }
};

class MemoryStream:public Stream{
public:
    virtual void read(){
        cout<<"This is MemoryStream Read"<<endl;
    }

    virtual void write(){
        cout<<"This is MemoryStream Write"<<endl;
    }

    virtual void seek(){
        cout<<"This is MemoryStream Seek"<<endl;
    }
};

class Decorate:public Stream{
    public:
    Decorate(Stream *_stream):stream(_stream){
        cout<<"This is Decoreate Stream"<<endl;
    }
    protected:
    Stream *stream;
};

class CytroStream:public Decorate{//加密的方法,可以针对所有的stream
    public:
        CytroStream(Stream *_stream):Decorate(_stream){
            cout<<"This is CytroStream"<<endl;
        }
        virtual void read(){
            cout<<"CytroStream read"<<endl;
            stream->read();
        }
        virtual void write(){
            cout<<"CytroStream write"<<endl;
            stream->write();
        }

        virtual void seek(){
            cout<<"CytroStream seek"<<endl;
            stream->write();
        }
};

class FileterStream:public Decorate{//过滤的方法,可以针对所有的stream
    public:
        FileterStream(Stream *_stream):Decorate(_stream){
            cout<<"This is FileterStream"<<endl;
        }
        virtual void read(){
            cout<<"FileterStream read"<<endl;
            stream->read();
        }
        virtual void write(){
            cout<<"FileterStream write"<<endl;
            stream->write();
        }

        virtual void seek(){
            cout<<"FileterStream seek"<<endl;
            stream->write();
        }
};


int main()
{
    FileStream *s1 = new FileStream();
    s1->read();
    cout<<"------------"<<endl;
    BufferStream *s2 = new BufferStream();
    s2->read();
    cout<<"------------"<<endl;
    //对filestream进行加密
    CytroStream *s3 = new CytroStream(s1);
    s3->read();
    cout<<"------------"<<endl;

    //对Bufferstream进行加密
    CytroStream *s4 = new CytroStream(s2);
    s4->read();
    cout<<"------------"<<endl;

    //对file进行过滤
    FileterStream *fs = new FileterStream(s1);
    fs->read();
    cout<<"------------"<<endl;
    //对filestream既加密有过滤
    CytroStream *s5  = new CytroStream(fs);
    s5->read();


    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    delete fs;
}

/*
    1. 装饰器（Decorator）模式的定义：指在不改变现有对象结构的情况下，
        动态地给该对象增加一些职责（即增加其额外功能）的模式，它属于对象结构型模式。
    2. 通常情况下，扩展一个类的功能会使用继承方式来实现。但继承具有静态特征，耦合度高，
        并且随着扩展功能的增多，子类会很膨胀。如果使用组合关系来创建一个包装对象（即装饰对象）
        来包裹真实对象，并在保持真实对象的类结构不变的前提下，为其提供额外的功能，这就是装饰器模式的目标。下面来分析其基本结构和实现方法。
    
    3. 模式的结构
        装饰器模式主要包含以下角色。
        抽象构件（Component）角色：定义一个抽象接口以规范准备接收附加责任的对象。
        具体构件（ConcreteComponent）角色：实现抽象构件，通过装饰角色为其添加一些职责。
        抽象装饰（Decorator）角色：继承抽象构件，并包含具体构件的实例，可以通过其子类扩展具体构件的功能。
        具体装饰（ConcreteDecorator）角色：实现抽象装饰的相关方法，并给具体构件对象添加附加的责任。
*/