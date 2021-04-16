#include <iostream>
using namespace std;
class MessageImpl{
    public:
        virtual void PlaySound()=0;
        virtual void DrawShape()=0;
        virtual void WriteText()=0;
        virtual void Connect()=0;
        
        virtual ~MessageImpl(){}
};

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
*/