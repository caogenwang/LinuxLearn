#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(){
    // string s;
    // cout<<"length: "<<s.length()<<endl;
    // vector<int> v;
    // cout<<"capacity: "<<v.capacity() <<endl;

    // s="hello";
    // cout<<"length: "<<s.length()<<endl;
    // cout<<"max: "<<s.max_size()<<endl;
    // s.push_back('c');
    // cout<<s<<endl;
    // string s2=s+" world!";
    // cout<<s2<<endl;

    // string str("cat,dog,cat,pig,little cat,hotdog,little pig,angry dog");
    // size_t catPos = str.find("cat",0);

    // if (catPos == string::npos) {
    //     printf("没有找到字符串\n");
    //     return 0;
    // }

    // while (catPos != string::npos) {
    //     cout << "在索引 " << catPos << " 处找到字符串" << endl;
    //     catPos = str.find("cat", catPos + 1);
    // }

    string str("Hello,World!");
    cout<<"length: "<<str.length()<<endl;
    cout<<"max: "<<str.max_size()<<endl;
    str.erase(5,6);                    // 删除从索引位置 5 开始的 6 个字符
    cout << "str 为：" << str << endl;
    cout<<"length: "<<str.length()<<endl;
    cout<<"max: "<<str.max_size()<<endl;

    //都是深拷贝
    char *name = "helloworld";
    string str2(name,8);
    cout<<str2<<endl;

    string str3(name);
    cout<<str3<<endl;

    string str4(10,'c');
    cout<<str4<<endl;

    string str5(str4);
    cout<<str5<<endl;

    cout<<"-------------"<<endl;
    string str6;
    str6 =  str5;
    cout<<str6<<endl;

    string str7;
    str7 = "helloworld";
    cout<<str7<<endl;

    string str8;
    str8 = 'd';
    cout<<str8<<endl;

    str8.resize(10,'a');//填充的参数可以直接赋0
    cout<<str8<<endl;

    str8.resize(20,'c');//填充的参数可以直接赋0
    cout<<str8<<endl;
    str8.push_back('f');
    cout<<str8<<endl;
    str8.pop_back();
    cout<<str8<<endl;
    return 0;
}
/*
三种赋值
basic_string& operator=(const basic_string& __s) {
    if (&__s != this) 
      assign(__s.begin(), __s.end());//copy赋值
    return *this;
  }

  basic_string& operator=(const _CharT* __s) 
    { return assign(__s, __s + _Traits::length(__s)); }

  basic_string& operator=(_CharT __c)
    { return assign(static_cast<size_type>(1), __c); }

*/