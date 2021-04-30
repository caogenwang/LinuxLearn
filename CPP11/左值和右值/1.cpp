#include <iostream>
using namespace std;

class HasPtrMem{
public:
    HasPtrMem():d(new int(0)){
        cout<<"Construct: "<<++n_cstr<<endl;
    }
    HasPtrMem(const HasPtrMem &h):d(new int(*h.d)){
        cout<<"Copy construct: "<<++n_cptr<<endl;
    }
    HasPtrMem(HasPtrMem &&h):d(h.d){
        h.d = nullptr;
        cout<<"Move construct: "<<++n_cptr<<endl;
    }
    ~HasPtrMem(){
        cout<<"Destruct: "<<++n_dstr<<endl;
    }
    int *d;
    static int n_cstr;
    static int n_dstr;
    static int n_cptr;
};

int HasPtrMem::n_cstr = 0;
int HasPtrMem::n_dstr = 0;
int HasPtrMem::n_cptr = 0;

HasPtrMem GetTemp(){ 
    HasPtrMem h;
    cout<<"Resource from "<< __func__<<": "<<hex<<h.d<<endl;
    return h;
}

// void func(int a){
//     cout<<"not ref:"<<a<<endl;
// }

void func(const int & a){
    cout<<"ref:"<<a<<endl;
}
int main(){
    int a = 10;
    int &b = a;
    func(a);//这个地方传入a和b都可以
}

/*
右值引用是直接绑定到右值上，
引用的含义就是要绑定到具体对象上.

引用和值不能构成重载
*/