//指向单对象
template <class _Ty, class _Dx> //注意，删除器也是unique_ptr类型的一部分
class unique_ptr { // non-copyable pointer to an object
private:
    _Compressed_pair<_Dx, pointer> _Mypair;
public:

    using pointer      = _Ty*;//裸指针类型
    using element_type = _Ty; //对象类型
    using deleter_type = _Dx; //删除器类型

    template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr() noexcept : _Mypair(_Zero_then_variadic_args_t()) {} //构造一个空的智能指针

    unique_ptr& operator=(nullptr_t) noexcept; //重置指针为nullptr

    //注意，explicit阻止隐式构造,如unique_ptr<int> up = new int(100);编译错误。只能显示构造，如unique_ptr<int> up(new int(100));
    template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
    explicit unique_ptr(pointer _Ptr) noexcept : _Mypair(_Zero_then_variadic_args_t(), _Ptr) {} 

    template <class _Dx2 = _Dx, enable_if_t<is_constructible_v<_Dx2, const _Dx2&>, int> = 0>
    unique_ptr(pointer _Ptr, const _Dx& _Dt) noexcept : _Mypair(_One_then_variadic_args_t(), _Dt, _Ptr) {}

    unique_ptr(unique_ptr&& _Right) noexcept;  //移动构造

    unique_ptr& operator=(unique_ptr&& _Right) noexcept;//移动赋值

    void swap(unique_ptr& _Right) noexcept;//交换两个智能指针所指向的对象

    ~unique_ptr() noexcept; //析构函数，调用删除器释放资源。

    Dx& get_deleter() noexcept; //返回删除器

    const _Dx& get_deleter() const noexcept;//返回删除器

    add_lvalue_reference_t<_Ty> operator*() const; //解引用

    pointer operator->() const noexcept; //智能指针->运算符

    pointer get() const noexcept; 

    explicit operator bool() const noexcept; //类型转换函数，用于条件语句，如if(uniptr)之类

    pointer release() noexcept; //返回裸指针，并释放所有权

    void reset(pointer _Ptr = pointer()) noexcept ; //重置指针,这个地方会析构原始指针

    unique_ptr(const unique_ptr&) = delete; //不可拷贝
    unique_ptr& operator=(const unique_ptr&) = delete; //不可拷贝赋值
};

//指向数组类型
template <class _Ty, class _Dx>
class unique_ptr<_Ty[], _Dx> { 
private:
    _Compressed_pair<_Dx, pointer> _Mypair; 
public:
    using pointer      = typename _Get_deleter_pointer_type<_Ty, remove_reference_t<_Dx>>::type;
    using element_type = _Ty;
    using deleter_type = _Dx;

    //...    //省略了与unique_ptr单对象类型相同的一些操作
   
    ~unique_ptr() noexcept; //析构函数，调用删除器释放资源。

    _Ty& operator[](size_t _Idx) const {  //数组[]操作符
        return _Mypair._Myval2[_Idx];
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
};