// Summary: C++ vector中的resize和reserve函数使用
// Author:  Amusi
// Date:    2018-07-18

#include <iostream>
#include <vector>

using namespace std;

int main(){

    vector<int> v1;
    vector<int> v2;
    vector<int> v3;
    vector<int> v4;

    cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity()<<endl;
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
    cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;    
    cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

    v2.push_back(99);
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;

    int nums = 20;
    for (int i = 0; i < nums; ++i){
        v2.push_back(i+1);
        cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
    }

    cout << endl;

    v1.resize(100);//填充0，扩充到100
    v2.resize(100);//填充0，扩充到100
    v2.push_back(99);   // 添加一个元素

    v3.reserve(100);//扩充100，但是不使用
    v4.reserve(100);//扩充100，但是不使用

    cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity() << endl;
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
    cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;
    cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

    v1.resize(200);//填充0，扩充到200
    v2.reserve(200);// 比capacity大，才会进行操作，扩充到200
    v3.reserve(200);//扩充，但是不使用
    v4.resize(200);//填充0，扩充到200

    cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity() << endl;
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
    cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;
    cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

    v4.push_back(8);

    cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity() << endl;
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
    cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;
    cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

    return 0;
}
/*
    size()：返回vector中的元素个数。
    capacity()：返回vector能存储元素的总数，reserve只有比他大的时候才会起作用，并且这部分内存不会被使用。
    resize()操作：创建指定数量的的元素并指定vector的存储空间，没有给值，则会默认一个空值，调用无参构造。
    reserve()操作：指定vector的元素总数。就是当前申请空间的总数。
*/