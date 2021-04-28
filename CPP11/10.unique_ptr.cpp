#include<iostream>
#include<memory>
using namespace std;

class Student{
public:
    Student(){
        cout<<"student"<<endl;
    }
    Student(string _name,int _age):name(_name),age(_age){
        cout<<"name:"<<name<<" age:"<<age<<endl;
    }
    ~Student(){
        cout<<"~student"<<endl;
    }
    void print() const
    {
        std::cout << "name:" << name << " age:" << age <<std::endl;
    }
    string name;
    int age;

};

class DeleteF{
    public:
    void operator()(Student *s){
        cout<<"I am a functioner"<<endl;
        delete []s;//注意，这个地方针对数组的，如果不是数组可以直接是   delete s
    }
};

void deletef(Student *s){
     cout<<"deletef"<<endl;
    delete s;
}

int main(){
    // int *a = new int(20);
    // string *psa = new string[10];
    // int *b = new int[10];
    // for (int i = 0; i < 15; i++)
    // {
    //     *b=i;
    //     b++;
    // }

    // Student *s = new Student[2]{Student("xiao hong",50),Student("hua wei",100)};//自定义类型的初始化
    // s->print();
    // (s+1)->print();
    // std::unique_ptr<Student> ups1(new Student("xiaoming",10));
    /*智能指针指向数组*/
    // std::unique_ptr<Student[]> ups(new Student[2]{Student("xiao hong",50),Student("hua wei",100)});

    cout<<"------分割线------"<<endl;
    /*带有删除器的智能指针,析构的时候会调用你传入的function*/
    // DeleteF d;
    // std::unique_ptr<Student,DeleteF> ups3(new Student("lihua",15),d);

    /*智能指针指向数组*/
    // std::unique_ptr<Student[],DeleteF> ups(new Student[2]{Student("xiao hong",50),Student("hua wei",100)},d);

    std::unique_ptr<Student,decltype(&deletef)> ups3(new Student("lihua",15),deletef);//如此定义，，可以使用普通函数作为删除器
    // delete []s;//这个指针必须定位到开始，才能全部删除，否则出错
    return 0;
}