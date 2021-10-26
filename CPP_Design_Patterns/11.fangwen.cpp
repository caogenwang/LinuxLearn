#include <iostream>
#include <string>
#include <list>
using namespace std;

class Elelmet;
class Visitor{
    public:
        virtual void display(Elelmet *e){};
};

class Elelmet{
    public:
        virtual void accept(Visitor v){};
};

class Employee:public Elelmet{
    public:
        Employee(int _age,string _name,string _hobby):
        age(_age),name(_name),hobby(_hobby)
        {}
        virtual void accept(Visitor *v){
            v->display(this);
        }
    int age;
    string name;
    string hobby;
};

class NameVisitor:public Visitor{
    public:
        virtual void display(Elelmet *e){
            Employee *ee = (Employee *)e;
            cout<<ee->name<<endl;
        }
};

class AgeVisitor:public Visitor{
    public:
        virtual void display(Elelmet *e){
            Employee *ee = (Employee *)e;
            cout<<ee->age<<endl;
        }
};

class HobbyVisitor:public Visitor{
    public:
        virtual void display(Elelmet *e){
            Employee *ee = (Employee *)e;
            cout<<ee->hobby<<endl;
        }
};

class Employees{
    public:
    void Add(Employee e){
        employees.push_back(e);
    }
    void accept(Visitor *v){
        for(list<Employee>::iterator it=employees.begin();it != employees.end();it++)
        {
            (*it).accept(v);
        }
    }
    list<Employee> employees;
};

int main(){
    Employees es;
    Employee e1(18,"xiaowang","zuqiu");
    Employee e2(19,"xiaoliu","lanqiu");
    Employee e3(20,"xiaohua","pingpang");
    es.Add(e1);es.Add(e2);es.Add(e3);

    HobbyVisitor *h = new HobbyVisitor();
    NameVisitor *n = new NameVisitor();//必须用指针
    es.accept(h);
    es.accept(n);
}