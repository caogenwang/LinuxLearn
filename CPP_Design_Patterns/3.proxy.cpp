class ISubject{
public:
    virtual void process();
};


class RealSubject: public ISubject{
public:
    virtual void process(){
        //....
    }
};

class ClientApp{
    
    ISubject* subject;
    
public:
    
    ClientApp(){
        subject=new RealSubject();
    }
    
    void DoTask(){
        //...
        subject->process();
        
        //....
    }
};

/*
代理模式就是找一个中间的代理商作为中介，是用户不直接生成真正的对象；
中间商负责生成具体的对象
*/