//数据结构中的元素是稳定的，但是经常要方法变动，需要将算法和结构进行分离

#include <iostream>
#include <string>
#include <list>
using namespace std;

class Element;

class Visitor//Visitor是不同的，不同的访问方法
{
public:
    virtual void Visit( Element *element ){};//具体的元素是固定的
};

// "Element"
class Element
{
public:
    // Methods
    virtual void Accept( Visitor *visitor ){};
};


// "ConcreteElement"
class Employee : public Element
{
public:
    string name;
    double income;
    int vacationDays;

public :
    Employee( string name, double income,
        int vacationDays )
    {
        this->name = name;
        this->income = income;
        this->vacationDays = vacationDays;
    }

    void Accept( Visitor *visitor )//可以接收很多不同的visitor
    {
        visitor->Visit( this );
    }
};

class IncomeVisitor : public Visitor//不同的visitor
{
public:    
    void Visit( Element *element )
    {
        Employee *employee = ((Employee*)element);
        employee->income *= 1.10;
        cout<<employee->name<<" 's new income: " <<employee->income<<endl;

    }
};

class VacationVisitor : public Visitor//不同的visitor
{
public :
    void Visit( Element *element )
    {
        Employee *employee = ((Employee*)element);//做了具体类型的强转了
        // Provide 3 extra vacation days
        employee->vacationDays += 3;        
        cout<<employee->name<<" 's new vacation days: " <<employee->income<<endl;
    }
};

// "ObjectStructure"
class Employees
{    
private :
    list< Employee*> employees;

public :

    void Attach( Employee *employee )
    {        
        employees.push_back(employee);        
    }

    void Detach( Employee *employee )
    {
        employees.remove(employee);        
    }

    void Accept( Visitor *visitor )
    {        
        for (std::list<Employee*>::iterator it=employees.begin(); it != employees.end(); ++it)
            (*it)->Accept(visitor);
    }
};

int main( )
{
    Employees *e = new Employees();
    e->Attach( new Employee( "Tom", 25000.0, 14 ) );
    e->Attach( new Employee( "Thomas", 35000.0, 16 ) );
    e->Attach( new Employee( "Roy", 45000.0, 21 ) );

    // Create two visitors
    IncomeVisitor *v1 = new IncomeVisitor();
    VacationVisitor *v2 = new VacationVisitor();//

    // Employees are visited
    e->Accept( v1 );//v1中的visitor函数去访问e中的对象
    e->Accept( v2 );
    return 0;    
}

