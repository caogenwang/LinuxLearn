#include <iostream>
using namespace std;

/*业务抽象,是不同平台需要具体实现的*/
class MessageImpl{
    public:
        virtual void PlaySound()=0;
        virtual void DrawShape()=0;
        virtual void WriteText()=0;
        virtual void Connect()=0;
        
        virtual ~MessageImpl(){}
};
/*平台部分是公共部分*/
class Mesage{
    protected:
    MessageImpl* messageImp;
    public:
        Mesage(MessageImpl* _messageImp):messageImp(_messageImp){}
        virtual void Login() = 0;
        virtual void SendMessage() = 0;
        virtual void SendPicture()=0;
        virtual ~Mesage(){}
};

/*平台实现*/
class PCMessagerImp : public MessageImpl{
public:
    virtual void PlaySound(){
        cout<<"PCMessagerImp PlaySound"<<endl;
    }
    virtual void DrawShape(){
         cout<<"PCMessagerImp DrawShape"<<endl;
    }
    virtual void WriteText(){
         cout<<"PCMessagerImp WriteText"<<endl;
    }
    virtual void Connect(){
        cout<<"PCMessagerImp Connect"<<endl;
    }
};

class MobileMessageImp:public MessageImpl{
public:
    virtual void PlaySound(){
        cout<<"MobileMessageImp PlaySound"<<endl;
    }
    virtual void DrawShape(){
         cout<<"MobileMessageImp DrawShape"<<endl;
    }
    virtual void WriteText(){
         cout<<"MobileMessageImp WriteText"<<endl;
    }
    virtual void Connect(){
        cout<<"MobileMessageImp Connect"<<endl;
    }
};

class MessagerLite :public Mesage {
public:
    MessagerLite(MessageImpl*_mesageiml):Mesage(_mesageiml){}
    virtual void Login(){
        messageImp->Connect();
    }
    virtual void SendMessage(){
        messageImp->WriteText();
    }
    virtual void SendPicture(){
        messageImp->DrawShape();
    }
};

class MessagerPerfect  :public Mesage {
public:
    MessagerPerfect(MessageImpl*_mesageiml):Mesage(_mesageiml){}
    virtual void Login() {
        messageImp->PlaySound();
        messageImp->Connect();
    }
    virtual void SendMessage(){
        messageImp->PlaySound();
        messageImp->WriteText();
    }
    virtual void SendPicture(){
        messageImp->PlaySound();
        messageImp->DrawShape();
    }
};

int main()
{
    //运行时装配
    MessageImpl* mImp=new PCMessagerImp();
    Mesage *m = new MessagerPerfect(mImp);
    m->Login();

    delete mImp;
    delete m;
    return 0;
}


/*
平台实现；
业务不同；
应该放在不同的类中；
还是要抽出可变分布和不可变部分；
抽象部分和实现部分进行剥离

继承转组合，与装饰模式不同的是，需要将原来放在同一个类中的功能进行拆分，所谓的桥应该指啥？桥接模式理解为将不同维度的变化作为独立的类，然后
用组合的方式放到一个类中，这个类中放的都是抽象类。

1. 桥接（Bridge）模式的定义如下：将抽象与实现分离，使它们可以独立变化。
    它是用组合关系代替继承关系来实现，从而降低了抽象和实现这两个可变维度的耦合度。

*/