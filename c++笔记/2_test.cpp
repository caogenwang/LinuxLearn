#include <iostream>
#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

class Value{
public:
    Value():ptr(nullptr){};
    ~Value(){
        cout<<"xigou"<<endl;
        if (ptr != nullptr){
            delete ptr;
        }
    }
    int *ptr;
};


class Objects{
    public:
    Value value;
};

typedef struct Object_s{
    int a;
    int b;
    Value v;
}Object_t;


int test(){
     //示例 2
    // Object_t *dev = (Object_t *)malloc(sizeof(Object_t));//当dev是指针的时候，v的析构函数不会被调用
    // dev->a = 1;                                          //当dev结构体的时候，V的析构函数会被调用
    // dev->b = 2;
    // dev->v.ptr = new int(5);
    // free(dev);


    //示例 2
    Objects *o2 = new Objects();
    delete o2;

    Objects o;//当类中包含其他类定义的对象时，析构时会调用析构函数

    return 0;
}

#ifdef __cplusplus
}
#endif