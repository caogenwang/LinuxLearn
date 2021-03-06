// unique_ptr implementation -*- C++ -*-

// Copyright (C) 2008-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/unique_ptr.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _UNIQUE_PTR_H
#define _UNIQUE_PTR_H 1

#include <bits/c++config.h>
#include <debug/debug.h>
#include <type_traits>
#include <utility>
#include <tuple>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup pointer_abstractions
   * @{
   */

#if _GLIBCXX_USE_DEPRECATED
  template<typename> class auto_ptr;
#endif

  /// Primary template of default_delete, used by unique_ptr
  template<typename _Tp>//删除单个元素
    struct default_delete
    {
      /// Default constructor
      constexpr default_delete() noexcept = default;

      /** @brief Converting constructor.
       *
       * Allows conversion from a deleter for arrays of another type, @p _Up,
       * only if @p _Up* is convertible to @p _Tp*.
       */
      template<typename _Up, typename = typename
	       enable_if<is_convertible<_Up*, _Tp*>::value>::type>
        default_delete(const default_delete<_Up>&) noexcept { }

      /// Calls @c delete @p __ptr
      void
      operator()(_Tp* __ptr) const
      {
	static_assert(!is_void<_Tp>::value,
		      "can't delete pointer to incomplete type");
	static_assert(sizeof(_Tp)>0,
		      "can't delete pointer to incomplete type");
	delete __ptr;//注意，对于自定义类的删除器的默认动作就是delete，这个时候会调用析构函数
      }
    };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 740 - omit specialization for array objects with a compile time length
  /// Specialization for arrays, default_delete.
  template<typename _Tp>
    struct default_delete<_Tp[]>
    {
    private:
      template<typename _Up>
	using __remove_cv = typename remove_cv<_Up>::type;

      // Like is_base_of<_Tp, _Up> but false if unqualified types are the same
      template<typename _Up>
	using __is_derived_Tp
	  = __and_< is_base_of<_Tp, _Up>,
		    __not_<is_same<__remove_cv<_Tp>, __remove_cv<_Up>>> >;

    public:
      /// Default constructor
      constexpr default_delete() noexcept = default;

      /** @brief Converting constructor.
       *
       * Allows conversion from a deleter for arrays of another type, such as
       * a const-qualified version of @p _Tp.
       *
       * Conversions from types derived from @c _Tp are not allowed because
       * it is unsafe to @c delete[] an array of derived types through a
       * pointer to the base type.
       */
      template<typename _Up, typename = typename
	       enable_if<!__is_derived_Tp<_Up>::value>::type>
        default_delete(const default_delete<_Up[]>&) noexcept { }

      /// Calls @c delete[] @p __ptr
      void
      operator()(_Tp* __ptr) const
      {
	static_assert(sizeof(_Tp)>0,
		      "can't delete pointer to incomplete type");
	delete [] __ptr;
      }

      template<typename _Up>
	typename enable_if<__is_derived_Tp<_Up>::value>::type
	operator()(_Up*) const = delete;
    };










  /// 20.7.1.2 unique_ptr for single objects.单个数据
  template <typename _Tp, typename _Dp = default_delete<_Tp> >//可以自定义一个删除器default_delete，仿函数啊
    class unique_ptr
    {
      // use SFINAE to determine whether _Del::pointer exists
      class _Pointer
      {
          template<typename _Up>
          static typename _Up::pointer __test(typename _Up::pointer*);//传入一个pointe*

          template<typename _Up>
          static _Tp* __test(...);//返回_Tp*的__test函数

          typedef typename remove_reference<_Dp>::type _Del;//对于模板类型，需要加typename

        public:
            typedef decltype(__test<_Del>(0)) type;//__test第一个类型_Tp*返回
      };

      typedef std::tuple<typename _Pointer::type, _Dp>  __tuple_type;
      __tuple_type                                      _M_t;//元组，存放的删除器类型和具体的删除器

    public:
      typedef typename _Pointer::type   pointer;//
      typedef _Tp                       element_type;//数据类型
      typedef _Dp                       deleter_type;//删除器类型

      // 默认的构造函数，不拥有任何东西
      /// Default constructor, creates a unique_ptr that owns nothing.
      /*
      noexcept
      该关键字告诉编译器，函数中不会发生异常,这有利于编译器对程序做更多的优化。
      如果在运行时，noexecpt函数向外抛出了异常（如果函数内部捕捉了异常并完成处理，
      这种情况不算抛出异常），程序会直接终止，调用std::terminate()函数，
      该函数内部会调用std::abort()终止程序。
      */
      constexpr unique_ptr() noexcept
      : _M_t()
      { static_assert(!is_pointer<deleter_type>::value,
		     "constructed with null function pointer deleter"); }

      /** Takes ownership of a pointer.
       *
       * @param __p  指向element_type对象的一个指针
       * @param __d  删除器的引用
       * The deleter will be value-initialized.
       */
      explicit
      unique_ptr(pointer __p) noexcept//
      : _M_t(__p, deleter_type())//
      { static_assert(!is_pointer<deleter_type>::value,
		     "constructed with null function pointer deleter"); }

      /** Takes ownership of a pointer.
       *
       * @param __p  A pointer to an object of @c element_type
       * @param __d  A reference to a deleter.
       *
       * The deleter will be initialized with @p __d
       */
      unique_ptr(pointer __p,
	  typename conditional<is_reference<deleter_type>::value,
	    deleter_type, const deleter_type&>::type __d) noexcept
      : _M_t(__p, __d) { }

      /** Takes ownership of a pointer.
       *
       * The deleter will be initialized with @p std::move(__d)
       */
      unique_ptr(pointer __p,
	  typename remove_reference<deleter_type>::type&& __d) noexcept
      : _M_t(std::move(__p), std::move(__d))
      { static_assert(!std::is_reference<deleter_type>::value,
		      "rvalue deleter bound to reference"); }

      /// Creates a unique_ptr that owns nothing.
      constexpr unique_ptr(nullptr_t) noexcept : unique_ptr() { }//委托构造函数，调用其他的构造函数


      /// 移动构造函数.右值引用
      /*
        此处注意两点：1.__u.release()返回裸指针，同时__u失去对裸指针的控制权。2.完美转发forward，根据推导的类型是左值引用还是右值引用，调用不同的函数 
      */
      unique_ptr(unique_ptr&& __u) noexcept
      : _M_t(__u.release(), std::forward<deleter_type>(__u.get_deleter())) { }


      /** @brief Converting constructor from another type
       *
       * Requires that the pointer owned by @p __u is convertible to the
       * type of pointer owned by this object, @p __u does not own an array,
       * and @p __u has a compatible deleter type.
       */
      template<typename _Up, typename _Ep, typename = _Require<
	       is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>,
	       __not_<is_array<_Up>>,
	       typename conditional<is_reference<_Dp>::value,
				    is_same<_Ep, _Dp>,
				    is_convertible<_Ep, _Dp>>::type>>
	unique_ptr(unique_ptr<_Up, _Ep>&& __u) noexcept
	: _M_t(__u.release(), std::forward<_Ep>(__u.get_deleter()))
	{ }

#if _GLIBCXX_USE_DEPRECATED
      /// Converting constructor from @c auto_ptr
      template<typename _Up, typename = _Require<
	       is_convertible<_Up*, _Tp*>, is_same<_Dp, default_delete<_Tp>>>>
	unique_ptr(auto_ptr<_Up>&& __u) noexcept;
#endif

      /// Destructor, invokes the deleter if the stored pointer is not null.
      ~unique_ptr() noexcept
      {
          auto& __ptr = std::get<0>(_M_t);
          if (__ptr != nullptr)
            get_deleter()(__ptr);//这里是删除器动作的地方
          __ptr = pointer();//析构指针
      }

      // Assignment.

      /** 移动赋值
       * @param __u  The object to transfer ownership from.
       * Invokes the deleter first if this object owns a pointer.
       */
      unique_ptr&operator=(unique_ptr&& __u) noexcept
      {
          reset(__u.release());
          get_deleter() = std::forward<deleter_type>(__u.get_deleter());
          return *this;
      }

      /** @brief Assignment from another type.
       *
       * @param __u  The object to transfer ownership from, which owns a
       *             convertible pointer to a non-array object.
       *
       * Invokes the deleter first if this object owns a pointer.
       */
      template<typename _Up, typename _Ep>
      typename enable_if< __and_<
        is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>,
        __not_<is_array<_Up>>
        >::value,
        unique_ptr&>::type
    operator=(unique_ptr<_Up, _Ep>&& __u) noexcept
    {
      reset(__u.release());
      get_deleter() = std::forward<_Ep>(__u.get_deleter());
      return *this;
    }

      /// 将unique_ptr置空
      unique_ptr& operator=(nullptr_t) noexcept
      {
        reset();//注意原始指针会被delete掉
        return *this;
      }

      // Observers.

      /// 解引用.
      typename add_lvalue_reference<element_type>::type
      operator*() const
      {
          _GLIBCXX_DEBUG_ASSERT(get() != pointer());
          return *get();//返回裸指针的值
      }

      /// Return the stored pointer.
      pointer operator->() const noexcept
      {
          _GLIBCXX_DEBUG_ASSERT(get() != pointer());
          return get();
      }

      /// 返回裸指针.
      pointer get() const noexcept
      { 
        return std::get<0>(_M_t); 
      }

      /// 返回删除器的引用
      deleter_type& get_deleter() noexcept
      { 
        return std::get<1>(_M_t); 
      }

      const deleter_type& get_deleter() const noexcept
      { 
        return std::get<1>(_M_t); 
      }

      /// 存贮的指针不为空的时候，返回true.
      explicit operator bool() const noexcept
      { 
        return get() == pointer() ? false : true; 
      }

      /// Release ownership of any stored pointer.
      pointer release() noexcept //注意，__p不会被析构，如果没有delete__p，会造成内存泄漏
      {
          pointer __p = get();
          std::get<0>(_M_t) = pointer();
          return __p;
      }

      void reset(pointer __p = pointer()) noexcept//默认传参是一个空指针
      {
          using std::swap;
          swap(std::get<0>(_M_t), __p);//注意这个地方，pointer()是一个空的时候，
          if (__p != pointer())        //智能指针失去控制权的时候，原始指针会被析构
            get_deleter()(__p);//!!!!!!
      }

      /// 智能指针的交换.
      void swap(unique_ptr& __u) noexcept
      {
          using std::swap;
          swap(_M_t, __u._M_t);
      }

      // 禁止拷贝构造和赋值构造.
      unique_ptr(const unique_ptr&) = delete;
      unique_ptr& operator=(const unique_ptr&) = delete;
  };














  /*智能指针指向一个数组*/
  /// 20.7.1.3 unique_ptr for array objects with a runtime length
  // [unique.ptr.runtime]
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 740 - omit specialization for array objects with a compile time length
  template<typename _Tp, typename _Dp>
    class unique_ptr<_Tp[], _Dp>//_Tp[]数组类型
    {
      // use SFINAE to determine whether _Del::pointer exists
      class _Pointer
      {
        template<typename _Up>
          static typename _Up::pointer __test(typename _Up::pointer*);

        template<typename _Up>
          static _Tp* __test(...);

        typedef typename remove_reference<_Dp>::type _Del;

            public:
        typedef decltype(__test<_Del>(0)) type;
      };

      typedef std::tuple<typename _Pointer::type, _Dp>  __tuple_type;
      __tuple_type                                      _M_t;

      template<typename _Up>
	using __remove_cv = typename remove_cv<_Up>::type;

      // like is_base_of<_Tp, _Up> but false if unqualified types are the same
      template<typename _Up>
	using __is_derived_Tp
	  = __and_< is_base_of<_Tp, _Up>,
		    __not_<is_same<__remove_cv<_Tp>, __remove_cv<_Up>>> >;

      template<typename _Up, typename _Ep,
	       typename _Tp_pointer = typename _Pointer::type,
	       typename _Up_pointer = typename unique_ptr<_Up, _Ep>::pointer>
	using __safe_conversion = __and_<
	    is_convertible<_Up_pointer, _Tp_pointer>,
	    is_array<_Up>,
	    __or_<__not_<is_pointer<_Up_pointer>>,
		  __not_<is_pointer<_Tp_pointer>>,
		  __not_<__is_derived_Tp<typename remove_extent<_Up>::type>>
	    >
	  >;

    public:
      typedef typename _Pointer::type	    pointer;
      typedef _Tp		 	                    element_type;
      typedef _Dp                         deleter_type;

      // Constructors.

      /// Default constructor, creates a unique_ptr that owns nothing.
      constexpr unique_ptr() noexcept
      : _M_t()
      { static_assert(!std::is_pointer<deleter_type>::value,
		      "constructed with null function pointer deleter"); }

      /** Takes ownership of a pointer.
       *
       * @param __p  A pointer to an array of @c element_type
       *
       * The deleter will be value-initialized.
       */
      explicit
      unique_ptr(pointer __p) noexcept
      : _M_t(__p, deleter_type())
      { static_assert(!is_pointer<deleter_type>::value,
		      "constructed with null function pointer deleter"); }

      // Disable construction from convertible pointer types.
      template<typename _Up, typename = _Require<is_pointer<pointer>,
	       is_convertible<_Up*, pointer>, __is_derived_Tp<_Up>>>
	    explicit unique_ptr(_Up* __p) = delete;

    
      unique_ptr(pointer __p,
	  typename conditional<is_reference<deleter_type>::value,
	      deleter_type, const deleter_type&>::type __d) noexcept
      : _M_t(__p, __d) { }

      /** Takes ownership of a pointer.
       *
       * The deleter will be initialized with @p std::move(__d)
       */
      unique_ptr(pointer __p, typename
		 remove_reference<deleter_type>::type&& __d) noexcept
      : _M_t(std::move(__p), std::move(__d))
      { static_assert(!is_reference<deleter_type>::value,
		      "rvalue deleter bound to reference"); }

      /// Move constructor.
      unique_ptr(unique_ptr&& __u) noexcept
      : _M_t(__u.release(), std::forward<deleter_type>(__u.get_deleter())) { }

      /// Creates a unique_ptr that owns nothing.
      constexpr unique_ptr(nullptr_t) noexcept : unique_ptr() { }

      template<typename _Up, typename _Ep,
	       typename = _Require<__safe_conversion<_Up, _Ep>,
		 typename conditional<is_reference<_Dp>::value,
				      is_same<_Ep, _Dp>,
				      is_convertible<_Ep, _Dp>>::type
	       >>
	unique_ptr(unique_ptr<_Up, _Ep>&& __u) noexcept
	: _M_t(__u.release(), std::forward<_Ep>(__u.get_deleter()))
	{ }

      /// Destructor, invokes the deleter if the stored pointer is not null.
      ~unique_ptr()
      {
          auto& __ptr = std::get<0>(_M_t);
          if (__ptr != nullptr)
            get_deleter()(__ptr);
          __ptr = pointer();
      }

      // Assignment.

      /** @brief Move assignment operator.
       *
       * @param __u  The object to transfer ownership from.
       *
       * Invokes the deleter first if this object owns a pointer.
       */
      unique_ptr&
      operator=(unique_ptr&& __u) noexcept
      {
        reset(__u.release());
        get_deleter() = std::forward<deleter_type>(__u.get_deleter());
        return *this;
      }

      /** @brief Assignment from another type.
       *
       * @param __u  The object to transfer ownership from, which owns a
       *             convertible pointer to an array object.
       *
       * Invokes the deleter first if this object owns a pointer.
       */
      template<typename _Up, typename _Ep>
	typename
	enable_if<__safe_conversion<_Up, _Ep>::value, unique_ptr&>::type
	operator=(unique_ptr<_Up, _Ep>&& __u) noexcept
	{
	  reset(__u.release());
	  get_deleter() = std::forward<_Ep>(__u.get_deleter());
	  return *this;
	}

      /// Reset the %unique_ptr to empty, invoking the deleter if necessary.
      unique_ptr& operator=(nullptr_t) noexcept
      {
          reset();
          return *this;
      }

      // Observers.

      /// Access an element of owned array.
      typename std::add_lvalue_reference<element_type>::type
      operator[](size_t __i) const//重载了中括号，可以像数组一样访问
      {
          _GLIBCXX_DEBUG_ASSERT(get() != pointer());
          return get()[__i];
      }

      /// Return the stored pointer.
      pointer get() const noexcept
      { 
        return std::get<0>(_M_t); 
      }

      /// Return a reference to the stored deleter.
      deleter_type& get_deleter() noexcept
      { 
        return std::get<1>(_M_t); 
      }

      /// Return a reference to the stored deleter.
      const deleter_type&
      get_deleter() const noexcept
      { return std::get<1>(_M_t); }

      /// Return @c true if the stored pointer is not null.
      explicit operator bool() const noexcept
      { return get() == pointer() ? false : true; }

      // Modifiers.

      /// Release ownership of any stored pointer.
      pointer release() noexcept
      {
          pointer __p = get();
          std::get<0>(_M_t) = pointer();
          return __p;
      }

      /** @brief Replace the stored pointer.
       *
       * @param __p  The new pointer to store.
       *
       * The deleter will be invoked if a pointer is already owned.
       */
      void reset(pointer __p = pointer()) noexcept
      {
            using std::swap;
            swap(std::get<0>(_M_t), __p);
            if (__p != nullptr)
              get_deleter()(__p);
      }

      // Disable resetting from convertible pointer types.
      template<typename _Up, typename = _Require<is_pointer<pointer>,is_convertible<_Up*, pointer>, __is_derived_Tp<_Up>>>
	    void reset(_Up*) = delete;

      /// Exchange the pointer and deleter with another object.
      void swap(unique_ptr& __u) noexcept
      {
        using std::swap;
        swap(_M_t, __u._M_t);
      }

      // Disable copy from lvalue.
      unique_ptr(const unique_ptr&) = delete;
      unique_ptr& operator=(const unique_ptr&) = delete;

      // Disable construction from convertible pointer types.
      template<typename _Up, typename = _Require<is_pointer<pointer>,is_convertible<_Up*, pointer>, __is_derived_Tp<_Up>>>
	    unique_ptr(_Up*, typename
		   conditional<is_reference<deleter_type>::value,
		   deleter_type, const deleter_type&>::type) = delete;

      // Disable construction from convertible pointer types.
      template<typename _Up, typename = _Require<is_pointer<pointer>,is_convertible<_Up*, pointer>, __is_derived_Tp<_Up>>>
	    unique_ptr(_Up*, typename
		  remove_reference<deleter_type>::type&&) = delete;
    };



  /*模板函数，处理unique_ptr智能指针相关*/

    template<typename _Tp, typename _Dp>
    inline void swap(unique_ptr<_Tp, _Dp>& __x,unique_ptr<_Tp, _Dp>& __y) noexcept
    { 
      __x.swap(__y); //最终调用的还是智能指针成员的函数，最终调用的是tuple的交换函数
    }

    template<typename _Tp, typename _Dp,typename _Up, typename _Ep>
    inline bool operator==(const unique_ptr<_Tp, _Dp>& __x,const unique_ptr<_Up, _Ep>& __y)
    { return __x.get() == __y.get(); }

    template<typename _Tp, typename _Dp>
    inline bool operator==(const unique_ptr<_Tp, _Dp>& __x, nullptr_t) noexcept
    { return !__x; }

    template<typename _Tp, typename _Dp>
    inline bool operator==(nullptr_t, const unique_ptr<_Tp, _Dp>& __x) noexcept
    { return !__x; }

    template<typename _Tp, typename _Dp,typename _Up, typename _Ep>
    inline bool operator!=(const unique_ptr<_Tp, _Dp>& __x,const unique_ptr<_Up, _Ep>& __y)
    { return __x.get() != __y.get(); }

    template<typename _Tp, typename _Dp>
    inline bool operator!=(const unique_ptr<_Tp, _Dp>& __x, nullptr_t) noexcept
    { return (bool)__x; }

    template<typename _Tp, typename _Dp>
    inline bool operator!=(nullptr_t, const unique_ptr<_Tp, _Dp>& __x) noexcept
    { return (bool)__x; }

  template<typename _Tp, typename _Dp,typename _Up, typename _Ep>
    inline bool operator<(const unique_ptr<_Tp, _Dp>& __x,const unique_ptr<_Up, _Ep>& __y)
    {
      typedef typename std::common_type<typename unique_ptr<_Tp, _Dp>::pointer,
      typename unique_ptr<_Up, _Ep>::pointer>::type _CT;
      return std::less<_CT>()(__x.get(), __y.get());
    }

    template<typename _Tp, typename _Dp>
    inline bool operator<(const unique_ptr<_Tp, _Dp>& __x, nullptr_t)
    { return std::less<typename unique_ptr<_Tp, _Dp>::pointer>()(__x.get(),nullptr); }

    template<typename _Tp, typename _Dp>
    inline bool operator<(nullptr_t, const unique_ptr<_Tp, _Dp>& __x)
    { return std::less<typename unique_ptr<_Tp, _Dp>::pointer>()(nullptr,__x.get()); }

    template<typename _Tp, typename _Dp,typename _Up, typename _Ep>
    inline bool
    operator<=(const unique_ptr<_Tp, _Dp>& __x,const unique_ptr<_Up, _Ep>& __y)
    { return !(__y < __x); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator<=(const unique_ptr<_Tp, _Dp>& __x, nullptr_t)
    { return !(nullptr < __x); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator<=(nullptr_t, const unique_ptr<_Tp, _Dp>& __x)
    { return !(__x < nullptr); }

  template<typename _Tp, typename _Dp,
	   typename _Up, typename _Ep>
    inline bool
    operator>(const unique_ptr<_Tp, _Dp>& __x,
	      const unique_ptr<_Up, _Ep>& __y)
    { return (__y < __x); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator>(const unique_ptr<_Tp, _Dp>& __x, nullptr_t)
    { return std::less<typename unique_ptr<_Tp, _Dp>::pointer>()(nullptr,
								 __x.get()); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator>(nullptr_t, const unique_ptr<_Tp, _Dp>& __x)
    { return std::less<typename unique_ptr<_Tp, _Dp>::pointer>()(__x.get(),
								 nullptr); }

  template<typename _Tp, typename _Dp,
	   typename _Up, typename _Ep>
    inline bool
    operator>=(const unique_ptr<_Tp, _Dp>& __x,
	       const unique_ptr<_Up, _Ep>& __y)
    { return !(__x < __y); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator>=(const unique_ptr<_Tp, _Dp>& __x, nullptr_t)
    { return !(__x < nullptr); }

  template<typename _Tp, typename _Dp>
    inline bool
    operator>=(nullptr_t, const unique_ptr<_Tp, _Dp>& __x)
    { return !(nullptr < __x); }

  /// std::hash specialization for unique_ptr.
  template<typename _Tp, typename _Dp>
    struct hash<unique_ptr<_Tp, _Dp>>
    : public __hash_base<size_t, unique_ptr<_Tp, _Dp>>
    {
      size_t
      operator()(const unique_ptr<_Tp, _Dp>& __u) const noexcept
      {
	typedef unique_ptr<_Tp, _Dp> _UP;
	return std::hash<typename _UP::pointer>()(__u.get());
      }
    };

#if __cplusplus > 201103L

#define __cpp_lib_make_unique 201304

  template<typename _Tp>
    struct _MakeUniq
    { typedef unique_ptr<_Tp> __single_object; };

  template<typename _Tp>
    struct _MakeUniq<_Tp[]>
    { typedef unique_ptr<_Tp[]> __array; };

  template<typename _Tp, size_t _Bound>
    struct _MakeUniq<_Tp[_Bound]>
    { struct __invalid_type { }; };

  ///模板函数
  /// std::make_unique for single objects
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__single_object
    make_unique(_Args&&... __args)
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }

  /// std::make_unique for arrays of unknown bound
  template<typename _Tp>
    inline typename _MakeUniq<_Tp>::__array
    make_unique(size_t __num)
    { return unique_ptr<_Tp>(new typename remove_extent<_Tp>::type[__num]()); }

  /// Disable std::make_unique for arrays of known bound
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__invalid_type
    make_unique(_Args&&...) = delete;
#endif

  // @} group pointer_abstractions

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _UNIQUE_PTR_H */
