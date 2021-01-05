#include <iostream>
#include <vector>
#include <alloca.h>
#ifdef __USE_MALLOC
typedef malloc_alloc alloc;//令第一级配置器为默认配置器
#else 
typedef __default_alloc_template<0> alloc;//令第二级为默认配置器
#endif
 
template<class T,class Alloc=alloc>
class vector
{
public:
	typedef T value_type;
protected:
	typedef simple_alloc<value_type,Alloc>  data_allocator;
};
 
 
typedef simple_alloc<int,alloc> int_new;
 
int main()
{
	vector<int> c;
	int* s=int_ne
	w::allocate(5);
     s[0]=0;s[1]=1;s[2]=2;s[3]=3;s[4]=4;
	 for(int i=0;i<5;++i){
		 std::cout<<s[i];
	 }
	int_new::deallocate(s,5);
	char c=getchar();
 
}