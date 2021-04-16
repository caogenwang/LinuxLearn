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

class CytroStream:public Stream{//加密的方法,可以针对所有的stream
    public:
        CytroStream(Stream *_stream):stream(_stream){
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
    private:
        Stream *stream; 
};

class FileterStream:public Stream{//过滤的方法,可以针对所有的stream
    public:
        FileterStream(Stream *_stream):stream(_stream){
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
    private:
        Stream *stream; 
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
    装饰模式符合单一职责的设计原则；
    解决方法继承中过于臃肿的问题；
    动态的给一个对象增加一些额外的职责，增加一些功能，可以不用通过继承的方式，消除重复代码减少子类个数
*/