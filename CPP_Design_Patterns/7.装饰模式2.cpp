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