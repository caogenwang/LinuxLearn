// -*- C++ -*-
//===--------------------------- atomic -----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_ATOMIC
#define _LIBCPP_ATOMIC

/*
    atomic synopsis

namespace std
{

// feature test macro

#define __cpp_lib_atomic_is_always_lock_free // as specified by SG10

// order and consistency

typedef enum memory_order
{
    memory_order_relaxed,
    memory_order_consume,  // load-consume
    memory_order_acquire,  // load-acquire
    memory_order_release,  // store-release
    memory_order_acq_rel,  // store-release load-acquire
    memory_order_seq_cst   // store-release load-acquire
} memory_order;

template <class T> T kill_dependency(T y) noexcept;

// lock-free property

#define ATOMIC_BOOL_LOCK_FREE unspecified
#define ATOMIC_CHAR_LOCK_FREE unspecified
#define ATOMIC_CHAR16_T_LOCK_FREE unspecified
#define ATOMIC_CHAR32_T_LOCK_FREE unspecified
#define ATOMIC_WCHAR_T_LOCK_FREE unspecified
#define ATOMIC_SHORT_LOCK_FREE unspecified
#define ATOMIC_INT_LOCK_FREE unspecified
#define ATOMIC_LONG_LOCK_FREE unspecified
#define ATOMIC_LLONG_LOCK_FREE unspecified
#define ATOMIC_POINTER_LOCK_FREE unspecified

// flag type and operations

typedef struct atomic_flag
{
    bool test_and_set(memory_order m = memory_order_seq_cst) volatile noexcept;
    bool test_and_set(memory_order m = memory_order_seq_cst) noexcept;
    void clear(memory_order m = memory_order_seq_cst) volatile noexcept;
    void clear(memory_order m = memory_order_seq_cst) noexcept;
    atomic_flag()  noexcept = default;
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) volatile = delete;
} atomic_flag;

bool
    atomic_flag_test_and_set(volatile atomic_flag* obj) noexcept;

bool
    atomic_flag_test_and_set(atomic_flag* obj) noexcept;

bool
    atomic_flag_test_and_set_explicit(volatile atomic_flag* obj,
                                      memory_order m) noexcept;

bool
    atomic_flag_test_and_set_explicit(atomic_flag* obj, memory_order m) noexcept;

void
    atomic_flag_clear(volatile atomic_flag* obj) noexcept;

void
    atomic_flag_clear(atomic_flag* obj) noexcept;

void
    atomic_flag_clear_explicit(volatile atomic_flag* obj, memory_order m) noexcept;

void
    atomic_flag_clear_explicit(atomic_flag* obj, memory_order m) noexcept;

#define ATOMIC_FLAG_INIT see below
#define ATOMIC_VAR_INIT(value) see below

template <class T>
struct atomic
{
    static constexpr bool is_always_lock_free;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(T desr, memory_order m = memory_order_seq_cst) volatile noexcept;
    void store(T desr, memory_order m = memory_order_seq_cst) noexcept;
    T load(memory_order m = memory_order_seq_cst) const volatile noexcept;
    T load(memory_order m = memory_order_seq_cst) const noexcept;
    operator T() const volatile noexcept;
    operator T() const noexcept;
    T exchange(T desr, memory_order m = memory_order_seq_cst) volatile noexcept;
    T exchange(T desr, memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(T& expc, T desr,
                               memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_weak(T& expc, T desr, memory_order s, memory_order f) noexcept;
    bool compare_exchange_strong(T& expc, T desr,
                                 memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_strong(T& expc, T desr,
                                 memory_order s, memory_order f) noexcept;
    bool compare_exchange_weak(T& expc, T desr,
                               memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_weak(T& expc, T desr,
                               memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_strong(T& expc, T desr,
                                memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_strong(T& expc, T desr,
                                 memory_order m = memory_order_seq_cst) noexcept;

    atomic() noexcept = default;
    constexpr atomic(T desr) noexcept;
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
    T operator=(T) volatile noexcept;
    T operator=(T) noexcept;
};

template <>
struct atomic<integral>
{
    static constexpr bool is_always_lock_free;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(integral desr, memory_order m = memory_order_seq_cst) volatile noexcept;
    void store(integral desr, memory_order m = memory_order_seq_cst) noexcept;
    integral load(memory_order m = memory_order_seq_cst) const volatile noexcept;
    integral load(memory_order m = memory_order_seq_cst) const noexcept;
    operator integral() const volatile noexcept;
    operator integral() const noexcept;
    integral exchange(integral desr,
                      memory_order m = memory_order_seq_cst) volatile noexcept;
    integral exchange(integral desr, memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(integral& expc, integral desr,
                               memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_weak(integral& expc, integral desr,
                               memory_order s, memory_order f) noexcept;
    bool compare_exchange_strong(integral& expc, integral desr,
                                 memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_strong(integral& expc, integral desr,
                                 memory_order s, memory_order f) noexcept;
    bool compare_exchange_weak(integral& expc, integral desr,
                               memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_weak(integral& expc, integral desr,
                               memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_strong(integral& expc, integral desr,
                                memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_strong(integral& expc, integral desr,
                                 memory_order m = memory_order_seq_cst) noexcept;

    integral
        fetch_add(integral op, memory_order m = memory_order_seq_cst) volatile noexcept;
    integral fetch_add(integral op, memory_order m = memory_order_seq_cst) noexcept;
    integral
        fetch_sub(integral op, memory_order m = memory_order_seq_cst) volatile noexcept;
    integral fetch_sub(integral op, memory_order m = memory_order_seq_cst) noexcept;
    integral
        fetch_and(integral op, memory_order m = memory_order_seq_cst) volatile noexcept;
    integral fetch_and(integral op, memory_order m = memory_order_seq_cst) noexcept;
    integral
        fetch_or(integral op, memory_order m = memory_order_seq_cst) volatile noexcept;
    integral fetch_or(integral op, memory_order m = memory_order_seq_cst) noexcept;
    integral
        fetch_xor(integral op, memory_order m = memory_order_seq_cst) volatile noexcept;
    integral fetch_xor(integral op, memory_order m = memory_order_seq_cst) noexcept;

    atomic() noexcept = default;
    constexpr atomic(integral desr) noexcept;
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
    integral operator=(integral desr) volatile noexcept;
    integral operator=(integral desr) noexcept;

    integral operator++(int) volatile noexcept;
    integral operator++(int) noexcept;
    integral operator--(int) volatile noexcept;
    integral operator--(int) noexcept;
    integral operator++() volatile noexcept;
    integral operator++() noexcept;
    integral operator--() volatile noexcept;
    integral operator--() noexcept;
    integral operator+=(integral op) volatile noexcept;
    integral operator+=(integral op) noexcept;
    integral operator-=(integral op) volatile noexcept;
    integral operator-=(integral op) noexcept;
    integral operator&=(integral op) volatile noexcept;
    integral operator&=(integral op) noexcept;
    integral operator|=(integral op) volatile noexcept;
    integral operator|=(integral op) noexcept;
    integral operator^=(integral op) volatile noexcept;
    integral operator^=(integral op) noexcept;
};

template <class T>
struct atomic<T*>
{
    static constexpr bool is_always_lock_free;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(T* desr, memory_order m = memory_order_seq_cst) volatile noexcept;
    void store(T* desr, memory_order m = memory_order_seq_cst) noexcept;
    T* load(memory_order m = memory_order_seq_cst) const volatile noexcept;
    T* load(memory_order m = memory_order_seq_cst) const noexcept;
    operator T*() const volatile noexcept;
    operator T*() const noexcept;
    T* exchange(T* desr, memory_order m = memory_order_seq_cst) volatile noexcept;
    T* exchange(T* desr, memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(T*& expc, T* desr,
                               memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_weak(T*& expc, T* desr,
                               memory_order s, memory_order f) noexcept;
    bool compare_exchange_strong(T*& expc, T* desr,
                                 memory_order s, memory_order f) volatile noexcept;
    bool compare_exchange_strong(T*& expc, T* desr,
                                 memory_order s, memory_order f) noexcept;
    bool compare_exchange_weak(T*& expc, T* desr,
                               memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_weak(T*& expc, T* desr,
                               memory_order m = memory_order_seq_cst) noexcept;
    bool compare_exchange_strong(T*& expc, T* desr,
                                memory_order m = memory_order_seq_cst) volatile noexcept;
    bool compare_exchange_strong(T*& expc, T* desr,
                                 memory_order m = memory_order_seq_cst) noexcept;
    T* fetch_add(ptrdiff_t op, memory_order m = memory_order_seq_cst) volatile noexcept;
    T* fetch_add(ptrdiff_t op, memory_order m = memory_order_seq_cst) noexcept;
    T* fetch_sub(ptrdiff_t op, memory_order m = memory_order_seq_cst) volatile noexcept;
    T* fetch_sub(ptrdiff_t op, memory_order m = memory_order_seq_cst) noexcept;

    atomic() noexcept = default;
    constexpr atomic(T* desr) noexcept;
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

    T* operator=(T*) volatile noexcept;
    T* operator=(T*) noexcept;
    T* operator++(int) volatile noexcept;
    T* operator++(int) noexcept;
    T* operator--(int) volatile noexcept;
    T* operator--(int) noexcept;
    T* operator++() volatile noexcept;
    T* operator++() noexcept;
    T* operator--() volatile noexcept;
    T* operator--() noexcept;
    T* operator+=(ptrdiff_t op) volatile noexcept;
    T* operator+=(ptrdiff_t op) noexcept;
    T* operator-=(ptrdiff_t op) volatile noexcept;
    T* operator-=(ptrdiff_t op) noexcept;
};


template <class T>
    bool
    atomic_is_lock_free(const volatile atomic<T>* obj) noexcept;

template <class T>
    bool
    atomic_is_lock_free(const atomic<T>* obj) noexcept;

template <class T>
    void
    atomic_init(volatile atomic<T>* obj, T desr) noexcept;

template <class T>
    void
    atomic_init(atomic<T>* obj, T desr) noexcept;

template <class T>
    void
    atomic_store(volatile atomic<T>* obj, T desr) noexcept;

template <class T>
    void
    atomic_store(atomic<T>* obj, T desr) noexcept;

template <class T>
    void
    atomic_store_explicit(volatile atomic<T>* obj, T desr, memory_order m) noexcept;

template <class T>
    void
    atomic_store_explicit(atomic<T>* obj, T desr, memory_order m) noexcept;

template <class T>
    T
    atomic_load(const volatile atomic<T>* obj) noexcept;

template <class T>
    T
    atomic_load(const atomic<T>* obj) noexcept;

template <class T>
    T
    atomic_load_explicit(const volatile atomic<T>* obj, memory_order m) noexcept;

template <class T>
    T
    atomic_load_explicit(const atomic<T>* obj, memory_order m) noexcept;

template <class T>
    T
    atomic_exchange(volatile atomic<T>* obj, T desr) noexcept;

template <class T>
    T
    atomic_exchange(atomic<T>* obj, T desr) noexcept;

template <class T>
    T
    atomic_exchange_explicit(volatile atomic<T>* obj, T desr, memory_order m) noexcept;

template <class T>
    T
    atomic_exchange_explicit(atomic<T>* obj, T desr, memory_order m) noexcept;

template <class T>
    bool
    atomic_compare_exchange_weak(volatile atomic<T>* obj, T* expc, T desr) noexcept;

template <class T>
    bool
    atomic_compare_exchange_weak(atomic<T>* obj, T* expc, T desr) noexcept;

template <class T>
    bool
    atomic_compare_exchange_strong(volatile atomic<T>* obj, T* expc, T desr) noexcept;

template <class T>
    bool
    atomic_compare_exchange_strong(atomic<T>* obj, T* expc, T desr) noexcept;

template <class T>
    bool
    atomic_compare_exchange_weak_explicit(volatile atomic<T>* obj, T* expc,
                                          T desr,
                                          memory_order s, memory_order f) noexcept;

template <class T>
    bool
    atomic_compare_exchange_weak_explicit(atomic<T>* obj, T* expc, T desr,
                                          memory_order s, memory_order f) noexcept;

template <class T>
    bool
    atomic_compare_exchange_strong_explicit(volatile atomic<T>* obj,
                                            T* expc, T desr,
                                            memory_order s, memory_order f) noexcept;

template <class T>
    bool
    atomic_compare_exchange_strong_explicit(atomic<T>* obj, T* expc,
                                            T desr,
                                            memory_order s, memory_order f) noexcept;

template <class Integral>
    Integral
    atomic_fetch_add(volatile atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_add(atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_add_explicit(volatile atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_add_explicit(atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_sub(volatile atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_sub(atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_sub_explicit(volatile atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_sub_explicit(atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_and(volatile atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_and(atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_and_explicit(volatile atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_and_explicit(atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_or(volatile atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_or(atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_or_explicit(volatile atomic<Integral>* obj, Integral op,
                             memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_or_explicit(atomic<Integral>* obj, Integral op,
                             memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_xor(volatile atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_xor(atomic<Integral>* obj, Integral op) noexcept;

template <class Integral>
    Integral
    atomic_fetch_xor_explicit(volatile atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;
template <class Integral>
    Integral
    atomic_fetch_xor_explicit(atomic<Integral>* obj, Integral op,
                              memory_order m) noexcept;

template <class T>
    T*
    atomic_fetch_add(volatile atomic<T*>* obj, ptrdiff_t op) noexcept;

template <class T>
    T*
    atomic_fetch_add(atomic<T*>* obj, ptrdiff_t op) noexcept;

template <class T>
    T*
    atomic_fetch_add_explicit(volatile atomic<T*>* obj, ptrdiff_t op,
                              memory_order m) noexcept;
template <class T>
    T*
    atomic_fetch_add_explicit(atomic<T*>* obj, ptrdiff_t op, memory_order m) noexcept;

template <class T>
    T*
    atomic_fetch_sub(volatile atomic<T*>* obj, ptrdiff_t op) noexcept;

template <class T>
    T*
    atomic_fetch_sub(atomic<T*>* obj, ptrdiff_t op) noexcept;

template <class T>
    T*
    atomic_fetch_sub_explicit(volatile atomic<T*>* obj, ptrdiff_t op,
                              memory_order m) noexcept;
template <class T>
    T*
    atomic_fetch_sub_explicit(atomic<T*>* obj, ptrdiff_t op, memory_order m) noexcept;

// Atomics for standard typedef types

typedef atomic<bool>               atomic_bool;
typedef atomic<char>               atomic_char;
typedef atomic<signed char>        atomic_schar;
typedef atomic<unsigned char>      atomic_uchar;
typedef atomic<short>              atomic_short;
typedef atomic<unsigned short>     atomic_ushort;
typedef atomic<int>                atomic_int;
typedef atomic<unsigned int>       atomic_uint;
typedef atomic<long>               atomic_long;
typedef atomic<unsigned long>      atomic_ulong;
typedef atomic<long long>          atomic_llong;
typedef atomic<unsigned long long> atomic_ullong;
typedef atomic<char16_t>           atomic_char16_t;
typedef atomic<char32_t>           atomic_char32_t;
typedef atomic<wchar_t>            atomic_wchar_t;

typedef atomic<int_least8_t>   atomic_int_least8_t;
typedef atomic<uint_least8_t>  atomic_uint_least8_t;
typedef atomic<int_least16_t>  atomic_int_least16_t;
typedef atomic<uint_least16_t> atomic_uint_least16_t;
typedef atomic<int_least32_t>  atomic_int_least32_t;
typedef atomic<uint_least32_t> atomic_uint_least32_t;
typedef atomic<int_least64_t>  atomic_int_least64_t;
typedef atomic<uint_least64_t> atomic_uint_least64_t;

typedef atomic<int_fast8_t>   atomic_int_fast8_t;
typedef atomic<uint_fast8_t>  atomic_uint_fast8_t;
typedef atomic<int_fast16_t>  atomic_int_fast16_t;
typedef atomic<uint_fast16_t> atomic_uint_fast16_t;
typedef atomic<int_fast32_t>  atomic_int_fast32_t;
typedef atomic<uint_fast32_t> atomic_uint_fast32_t;
typedef atomic<int_fast64_t>  atomic_int_fast64_t;
typedef atomic<uint_fast64_t> atomic_uint_fast64_t;

typedef atomic<int8_t>   atomic_int8_t;
typedef atomic<uint8_t>  atomic_uint8_t;
typedef atomic<int16_t>  atomic_int16_t;
typedef atomic<uint16_t> atomic_uint16_t;
typedef atomic<int32_t>  atomic_int32_t;
typedef atomic<uint32_t> atomic_uint32_t;
typedef atomic<int64_t>  atomic_int64_t;
typedef atomic<uint64_t> atomic_uint64_t;

typedef atomic<intptr_t>  atomic_intptr_t;
typedef atomic<uintptr_t> atomic_uintptr_t;
typedef atomic<size_t>    atomic_size_t;
typedef atomic<ptrdiff_t> atomic_ptrdiff_t;
typedef atomic<intmax_t>  atomic_intmax_t;
typedef atomic<uintmax_t> atomic_uintmax_t;

// fences

void atomic_thread_fence(memory_order m) noexcept;
void atomic_signal_fence(memory_order m) noexcept;

}  // std

*/

#include <__config>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

#ifdef _LIBCPP_HAS_NO_THREADS
#error <atomic> is not supported on this single threaded system
#endif
#if !defined(_LIBCPP_HAS_C_ATOMIC_IMP) && !defined(_LIBCPP_HAS_GCC_ATOMIC_IMP)
#error <atomic> is not implemented
#endif

#if _LIBCPP_STD_VER > 14
# define __cpp_lib_atomic_is_always_lock_free 201603L
#endif

#define _LIBCPP_CHECK_STORE_MEMORY_ORDER(__m) \
  _LIBCPP_DIAGNOSE_WARNING(__m == memory_order_consume || \
                           __m == memory_order_acquire || \
                           __m == memory_order_acq_rel,   \
                        "memory order argument to atomic operation is invalid")

#define _LIBCPP_CHECK_LOAD_MEMORY_ORDER(__m) \
  _LIBCPP_DIAGNOSE_WARNING(__m == memory_order_release || \
                           __m == memory_order_acq_rel,   \
                        "memory order argument to atomic operation is invalid")

#define _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__m, __f) \
  _LIBCPP_DIAGNOSE_WARNING(__f == memory_order_release || \
                           __f == memory_order_acq_rel,   \
                        "memory order argument to atomic operation is invalid")

_LIBCPP_BEGIN_NAMESPACE_STD

typedef enum memory_order
{
    memory_order_relaxed, memory_order_consume, memory_order_acquire,
    memory_order_release, memory_order_acq_rel, memory_order_seq_cst
} memory_order;

#if defined(_LIBCPP_HAS_GCC_ATOMIC_IMP)
namespace __gcc_atomic {
template <typename _Tp>
struct __gcc_atomic_t {

#if _GNUC_VER >= 501
    static_assert(is_trivially_copyable<_Tp>::value,
      "std::atomic<Tp> requires that 'Tp' be a trivially copyable type");
#endif

  _LIBCPP_INLINE_VISIBILITY
#ifndef _LIBCPP_CXX03_LANG
    __gcc_atomic_t() _NOEXCEPT = default;
#else
    __gcc_atomic_t() _NOEXCEPT : __a_value() {}
#endif // _LIBCPP_CXX03_LANG
  _LIBCPP_CONSTEXPR explicit __gcc_atomic_t(_Tp value) _NOEXCEPT
    : __a_value(value) {}
  _Tp __a_value;
};
#define _Atomic(x) __gcc_atomic::__gcc_atomic_t<x>

template <typename _Tp> _Tp __create();

template <typename _Tp, typename _Td>
typename enable_if<sizeof(_Tp()->__a_value = __create<_Td>()), char>::type
    __test_atomic_assignable(int);
template <typename _Tp, typename _Up>
__two __test_atomic_assignable(...);

template <typename _Tp, typename _Td>
struct __can_assign {
  static const bool value =
      sizeof(__test_atomic_assignable<_Tp, _Td>(1)) == sizeof(char);
};

static inline _LIBCPP_CONSTEXPR int __to_gcc_order(memory_order __order) {
  // Avoid switch statement to make this a constexpr.
  return __order == memory_order_relaxed ? __ATOMIC_RELAXED:
         (__order == memory_order_acquire ? __ATOMIC_ACQUIRE:
          (__order == memory_order_release ? __ATOMIC_RELEASE:
           (__order == memory_order_seq_cst ? __ATOMIC_SEQ_CST:
            (__order == memory_order_acq_rel ? __ATOMIC_ACQ_REL:
              __ATOMIC_CONSUME))));
}

static inline _LIBCPP_CONSTEXPR int __to_gcc_failure_order(memory_order __order) {
  // Avoid switch statement to make this a constexpr.
  return __order == memory_order_relaxed ? __ATOMIC_RELAXED:
         (__order == memory_order_acquire ? __ATOMIC_ACQUIRE:
          (__order == memory_order_release ? __ATOMIC_RELAXED:
           (__order == memory_order_seq_cst ? __ATOMIC_SEQ_CST:
            (__order == memory_order_acq_rel ? __ATOMIC_ACQUIRE:
              __ATOMIC_CONSUME))));
}

} // namespace __gcc_atomic

template <typename _Tp>
static inline
typename enable_if<
    __gcc_atomic::__can_assign<volatile _Atomic(_Tp)*, _Tp>::value>::type
__c11_atomic_init(volatile _Atomic(_Tp)* __a,  _Tp __val) {
  __a->__a_value = __val;
}

template <typename _Tp>
static inline
typename enable_if<
    !__gcc_atomic::__can_assign<volatile _Atomic(_Tp)*, _Tp>::value &&
     __gcc_atomic::__can_assign<         _Atomic(_Tp)*, _Tp>::value>::type
__c11_atomic_init(volatile _Atomic(_Tp)* __a,  _Tp __val) {
  // [atomics.types.generic]p1 guarantees _Tp is trivially copyable. Because
  // the default operator= in an object is not volatile, a byte-by-byte copy
  // is required.
  volatile char* to = reinterpret_cast<volatile char*>(&__a->__a_value);
  volatile char* end = to + sizeof(_Tp);
  char* from = reinterpret_cast<char*>(&__val);
  while (to != end) {
    *to++ = *from++;
  }
}

template <typename _Tp>
static inline void __c11_atomic_init(_Atomic(_Tp)* __a,  _Tp __val) {
  __a->__a_value = __val;
}

static inline void __c11_atomic_thread_fence(memory_order __order) {
  __atomic_thread_fence(__gcc_atomic::__to_gcc_order(__order));
}

static inline void __c11_atomic_signal_fence(memory_order __order) {
  __atomic_signal_fence(__gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline void __c11_atomic_store(volatile _Atomic(_Tp)* __a,  _Tp __val,
                                      memory_order __order) {
  return __atomic_store(&__a->__a_value, &__val,
                        __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline void __c11_atomic_store(_Atomic(_Tp)* __a,  _Tp __val,
                                      memory_order __order) {
  __atomic_store(&__a->__a_value, &__val,
                 __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_load(volatile _Atomic(_Tp)* __a,
                                    memory_order __order) {
  _Tp __ret;
  __atomic_load(&__a->__a_value, &__ret,
                __gcc_atomic::__to_gcc_order(__order));
  return __ret;
}

template <typename _Tp>
static inline _Tp __c11_atomic_load(_Atomic(_Tp)* __a, memory_order __order) {
  _Tp __ret;
  __atomic_load(&__a->__a_value, &__ret,
                __gcc_atomic::__to_gcc_order(__order));
  return __ret;
}

template <typename _Tp>
static inline _Tp __c11_atomic_exchange(volatile _Atomic(_Tp)* __a,
                                        _Tp __value, memory_order __order) {
  _Tp __ret;
  __atomic_exchange(&__a->__a_value, &__value, &__ret,
                    __gcc_atomic::__to_gcc_order(__order));
  return __ret;
}

template <typename _Tp>
static inline _Tp __c11_atomic_exchange(_Atomic(_Tp)* __a, _Tp __value,
                                        memory_order __order) {
  _Tp __ret;
  __atomic_exchange(&__a->__a_value, &__value, &__ret,
                    __gcc_atomic::__to_gcc_order(__order));
  return __ret;
}

template <typename _Tp>
static inline bool __c11_atomic_compare_exchange_strong(
    volatile _Atomic(_Tp)* __a, _Tp* __expected, _Tp __value,
    memory_order __success, memory_order __failure) {
  return __atomic_compare_exchange(&__a->__a_value, __expected, &__value,
                                   false,
                                   __gcc_atomic::__to_gcc_order(__success),
                                   __gcc_atomic::__to_gcc_failure_order(__failure));
}

template <typename _Tp>
static inline bool __c11_atomic_compare_exchange_strong(
    _Atomic(_Tp)* __a, _Tp* __expected, _Tp __value, memory_order __success,
    memory_order __failure) {
  return __atomic_compare_exchange(&__a->__a_value, __expected, &__value,
                                   false,
                                   __gcc_atomic::__to_gcc_order(__success),
                                   __gcc_atomic::__to_gcc_failure_order(__failure));
}

template <typename _Tp>
static inline bool __c11_atomic_compare_exchange_weak(
    volatile _Atomic(_Tp)* __a, _Tp* __expected, _Tp __value,
    memory_order __success, memory_order __failure) {
  return __atomic_compare_exchange(&__a->__a_value, __expected, &__value,
                                   true,
                                   __gcc_atomic::__to_gcc_order(__success),
                                   __gcc_atomic::__to_gcc_failure_order(__failure));
}

template <typename _Tp>
static inline bool __c11_atomic_compare_exchange_weak(
    _Atomic(_Tp)* __a, _Tp* __expected, _Tp __value, memory_order __success,
    memory_order __failure) {
  return __atomic_compare_exchange(&__a->__a_value, __expected, &__value,
                                   true,
                                   __gcc_atomic::__to_gcc_order(__success),
                                   __gcc_atomic::__to_gcc_failure_order(__failure));
}

template <typename _Tp>
struct __skip_amt { enum {value = 1}; };

template <typename _Tp>
struct __skip_amt<_Tp*> { enum {value = sizeof(_Tp)}; };

// FIXME: Haven't figured out what the spec says about using arrays with
// atomic_fetch_add. Force a failure rather than creating bad behavior.
template <typename _Tp>
struct __skip_amt<_Tp[]> { };
template <typename _Tp, int n>
struct __skip_amt<_Tp[n]> { };

template <typename _Tp, typename _Td>
static inline _Tp __c11_atomic_fetch_add(volatile _Atomic(_Tp)* __a,
                                         _Td __delta, memory_order __order) {
  return __atomic_fetch_add(&__a->__a_value, __delta * __skip_amt<_Tp>::value,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp, typename _Td>
static inline _Tp __c11_atomic_fetch_add(_Atomic(_Tp)* __a, _Td __delta,
                                         memory_order __order) {
  return __atomic_fetch_add(&__a->__a_value, __delta * __skip_amt<_Tp>::value,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp, typename _Td>
static inline _Tp __c11_atomic_fetch_sub(volatile _Atomic(_Tp)* __a,
                                         _Td __delta, memory_order __order) {
  return __atomic_fetch_sub(&__a->__a_value, __delta * __skip_amt<_Tp>::value,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp, typename _Td>
static inline _Tp __c11_atomic_fetch_sub(_Atomic(_Tp)* __a, _Td __delta,
                                         memory_order __order) {
  return __atomic_fetch_sub(&__a->__a_value, __delta * __skip_amt<_Tp>::value,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_and(volatile _Atomic(_Tp)* __a,
                                         _Tp __pattern, memory_order __order) {
  return __atomic_fetch_and(&__a->__a_value, __pattern,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_and(_Atomic(_Tp)* __a,
                                         _Tp __pattern, memory_order __order) {
  return __atomic_fetch_and(&__a->__a_value, __pattern,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_or(volatile _Atomic(_Tp)* __a,
                                        _Tp __pattern, memory_order __order) {
  return __atomic_fetch_or(&__a->__a_value, __pattern,
                           __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_or(_Atomic(_Tp)* __a, _Tp __pattern,
                                        memory_order __order) {
  return __atomic_fetch_or(&__a->__a_value, __pattern,
                           __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_xor(volatile _Atomic(_Tp)* __a,
                                         _Tp __pattern, memory_order __order) {
  return __atomic_fetch_xor(&__a->__a_value, __pattern,
                            __gcc_atomic::__to_gcc_order(__order));
}

template <typename _Tp>
static inline _Tp __c11_atomic_fetch_xor(_Atomic(_Tp)* __a, _Tp __pattern,
                                         memory_order __order) {
  return __atomic_fetch_xor(&__a->__a_value, __pattern,
                            __gcc_atomic::__to_gcc_order(__order));
}
#endif // _LIBCPP_HAS_GCC_ATOMIC_IMP

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
kill_dependency(_Tp __y) _NOEXCEPT
{
    return __y;
}

#if defined(__CLANG_ATOMIC_BOOL_LOCK_FREE)
# define ATOMIC_BOOL_LOCK_FREE      __CLANG_ATOMIC_BOOL_LOCK_FREE
# define ATOMIC_CHAR_LOCK_FREE      __CLANG_ATOMIC_CHAR_LOCK_FREE
# define ATOMIC_CHAR16_T_LOCK_FREE  __CLANG_ATOMIC_CHAR16_T_LOCK_FREE
# define ATOMIC_CHAR32_T_LOCK_FREE  __CLANG_ATOMIC_CHAR32_T_LOCK_FREE
# define ATOMIC_WCHAR_T_LOCK_FREE   __CLANG_ATOMIC_WCHAR_T_LOCK_FREE
# define ATOMIC_SHORT_LOCK_FREE     __CLANG_ATOMIC_SHORT_LOCK_FREE
# define ATOMIC_INT_LOCK_FREE       __CLANG_ATOMIC_INT_LOCK_FREE
# define ATOMIC_LONG_LOCK_FREE      __CLANG_ATOMIC_LONG_LOCK_FREE
# define ATOMIC_LLONG_LOCK_FREE     __CLANG_ATOMIC_LLONG_LOCK_FREE
# define ATOMIC_POINTER_LOCK_FREE   __CLANG_ATOMIC_POINTER_LOCK_FREE
#else
# define ATOMIC_BOOL_LOCK_FREE      __GCC_ATOMIC_BOOL_LOCK_FREE
# define ATOMIC_CHAR_LOCK_FREE      __GCC_ATOMIC_CHAR_LOCK_FREE
# define ATOMIC_CHAR16_T_LOCK_FREE  __GCC_ATOMIC_CHAR16_T_LOCK_FREE
# define ATOMIC_CHAR32_T_LOCK_FREE  __GCC_ATOMIC_CHAR32_T_LOCK_FREE
# define ATOMIC_WCHAR_T_LOCK_FREE   __GCC_ATOMIC_WCHAR_T_LOCK_FREE
# define ATOMIC_SHORT_LOCK_FREE     __GCC_ATOMIC_SHORT_LOCK_FREE
# define ATOMIC_INT_LOCK_FREE       __GCC_ATOMIC_INT_LOCK_FREE
# define ATOMIC_LONG_LOCK_FREE      __GCC_ATOMIC_LONG_LOCK_FREE
# define ATOMIC_LLONG_LOCK_FREE     __GCC_ATOMIC_LLONG_LOCK_FREE
# define ATOMIC_POINTER_LOCK_FREE   __GCC_ATOMIC_POINTER_LOCK_FREE
#endif

// general atomic<T>

template <class _Tp, bool = is_integral<_Tp>::value && !is_same<_Tp, bool>::value>
struct __atomic_base  // false
{
    mutable _Atomic(_Tp) __a_;

#if defined(__cpp_lib_atomic_is_always_lock_free)
  static _LIBCPP_CONSTEXPR bool is_always_lock_free = __atomic_always_lock_free(sizeof(__a_), 0);
#endif

    _LIBCPP_INLINE_VISIBILITY
    bool is_lock_free() const volatile _NOEXCEPT
    {
#if defined(_LIBCPP_HAS_C_ATOMIC_IMP)
    return __c11_atomic_is_lock_free(sizeof(_Tp));
#else
    return __atomic_is_lock_free(sizeof(_Tp), 0);
#endif
    }
    _LIBCPP_INLINE_VISIBILITY
    bool is_lock_free() const _NOEXCEPT
        {return static_cast<__atomic_base const volatile*>(this)->is_lock_free();}
    _LIBCPP_INLINE_VISIBILITY
    void store(_Tp __d, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
      _LIBCPP_CHECK_STORE_MEMORY_ORDER(__m)
        {__c11_atomic_store(&__a_, __d, __m);}
    _LIBCPP_INLINE_VISIBILITY
    void store(_Tp __d, memory_order __m = memory_order_seq_cst) _NOEXCEPT
      _LIBCPP_CHECK_STORE_MEMORY_ORDER(__m)
        {__c11_atomic_store(&__a_, __d, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp load(memory_order __m = memory_order_seq_cst) const volatile _NOEXCEPT
      _LIBCPP_CHECK_LOAD_MEMORY_ORDER(__m)
        {return __c11_atomic_load(&__a_, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp load(memory_order __m = memory_order_seq_cst) const _NOEXCEPT
      _LIBCPP_CHECK_LOAD_MEMORY_ORDER(__m)
        {return __c11_atomic_load(&__a_, __m);}
    _LIBCPP_INLINE_VISIBILITY
    operator _Tp() const volatile _NOEXCEPT {return load();}
    _LIBCPP_INLINE_VISIBILITY
    operator _Tp() const _NOEXCEPT          {return load();}
    _LIBCPP_INLINE_VISIBILITY
    _Tp exchange(_Tp __d, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_exchange(&__a_, __d, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp exchange(_Tp __d, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_exchange(&__a_, __d, __m);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_weak(_Tp& __e, _Tp __d,
                               memory_order __s, memory_order __f) volatile _NOEXCEPT
      _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
        {return __c11_atomic_compare_exchange_weak(&__a_, &__e, __d, __s, __f);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_weak(_Tp& __e, _Tp __d,
                               memory_order __s, memory_order __f) _NOEXCEPT
      _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
        {return __c11_atomic_compare_exchange_weak(&__a_, &__e, __d, __s, __f);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_strong(_Tp& __e, _Tp __d,
                                 memory_order __s, memory_order __f) volatile _NOEXCEPT
      _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
        {return __c11_atomic_compare_exchange_strong(&__a_, &__e, __d, __s, __f);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_strong(_Tp& __e, _Tp __d,
                                 memory_order __s, memory_order __f) _NOEXCEPT
      _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
        {return __c11_atomic_compare_exchange_strong(&__a_, &__e, __d, __s, __f);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_weak(_Tp& __e, _Tp __d,
                              memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_compare_exchange_weak(&__a_, &__e, __d, __m, __m);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_weak(_Tp& __e, _Tp __d,
                               memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_compare_exchange_weak(&__a_, &__e, __d, __m, __m);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_strong(_Tp& __e, _Tp __d,
                              memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_compare_exchange_strong(&__a_, &__e, __d, __m, __m);}
    _LIBCPP_INLINE_VISIBILITY
    bool compare_exchange_strong(_Tp& __e, _Tp __d,
                                 memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_compare_exchange_strong(&__a_, &__e, __d, __m, __m);}

    _LIBCPP_INLINE_VISIBILITY
#ifndef _LIBCPP_CXX03_LANG
    __atomic_base() _NOEXCEPT = default;
#else
    __atomic_base() _NOEXCEPT : __a_() {}
#endif // _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR __atomic_base(_Tp __d) _NOEXCEPT : __a_(__d) {}
#ifndef _LIBCPP_CXX03_LANG
    __atomic_base(const __atomic_base&) = delete;
    __atomic_base& operator=(const __atomic_base&) = delete;
    __atomic_base& operator=(const __atomic_base&) volatile = delete;
#else
private:
    __atomic_base(const __atomic_base&);
    __atomic_base& operator=(const __atomic_base&);
    __atomic_base& operator=(const __atomic_base&) volatile;
#endif
};

#if defined(__cpp_lib_atomic_is_always_lock_free)
template <class _Tp, bool __b>
_LIBCPP_CONSTEXPR bool __atomic_base<_Tp, __b>::is_always_lock_free;
#endif

// atomic<Integral>

template <class _Tp>
struct __atomic_base<_Tp, true>
    : public __atomic_base<_Tp, false>
{
    typedef __atomic_base<_Tp, false> __base;
    _LIBCPP_INLINE_VISIBILITY
    __atomic_base() _NOEXCEPT _LIBCPP_DEFAULT
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR __atomic_base(_Tp __d) _NOEXCEPT : __base(__d) {}

    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_add(_Tp __op, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_fetch_add(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_add(_Tp __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_add(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_sub(_Tp __op, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_fetch_sub(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_sub(_Tp __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_sub(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_and(_Tp __op, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_fetch_and(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_and(_Tp __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_and(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_or(_Tp __op, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_fetch_or(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_or(_Tp __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_or(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_xor(_Tp __op, memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_fetch_xor(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp fetch_xor(_Tp __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_xor(&this->__a_, __op, __m);}

    _LIBCPP_INLINE_VISIBILITY
    _Tp operator++(int) volatile _NOEXCEPT      {return fetch_add(_Tp(1));}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator++(int) _NOEXCEPT               {return fetch_add(_Tp(1));}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator--(int) volatile _NOEXCEPT      {return fetch_sub(_Tp(1));}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator--(int) _NOEXCEPT               {return fetch_sub(_Tp(1));}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator++() volatile _NOEXCEPT         {return fetch_add(_Tp(1)) + _Tp(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator++() _NOEXCEPT                  {return fetch_add(_Tp(1)) + _Tp(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator--() volatile _NOEXCEPT         {return fetch_sub(_Tp(1)) - _Tp(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator--() _NOEXCEPT                  {return fetch_sub(_Tp(1)) - _Tp(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator+=(_Tp __op) volatile _NOEXCEPT {return fetch_add(__op) + __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator+=(_Tp __op) _NOEXCEPT          {return fetch_add(__op) + __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator-=(_Tp __op) volatile _NOEXCEPT {return fetch_sub(__op) - __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator-=(_Tp __op) _NOEXCEPT          {return fetch_sub(__op) - __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator&=(_Tp __op) volatile _NOEXCEPT {return fetch_and(__op) & __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator&=(_Tp __op) _NOEXCEPT          {return fetch_and(__op) & __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator|=(_Tp __op) volatile _NOEXCEPT {return fetch_or(__op) | __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator|=(_Tp __op) _NOEXCEPT          {return fetch_or(__op) | __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator^=(_Tp __op) volatile _NOEXCEPT {return fetch_xor(__op) ^ __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator^=(_Tp __op) _NOEXCEPT          {return fetch_xor(__op) ^ __op;}
};

// atomic<T>

template <class _Tp>
struct atomic
    : public __atomic_base<_Tp>
{
    typedef __atomic_base<_Tp> __base;
    _LIBCPP_INLINE_VISIBILITY
    atomic() _NOEXCEPT _LIBCPP_DEFAULT
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR atomic(_Tp __d) _NOEXCEPT : __base(__d) {}

    _LIBCPP_INLINE_VISIBILITY
    _Tp operator=(_Tp __d) volatile _NOEXCEPT
        {__base::store(__d); return __d;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator=(_Tp __d) _NOEXCEPT
        {__base::store(__d); return __d;}
};

// atomic<T*>

template <class _Tp>
struct atomic<_Tp*>
    : public __atomic_base<_Tp*>
{
    typedef __atomic_base<_Tp*> __base;
    _LIBCPP_INLINE_VISIBILITY
    atomic() _NOEXCEPT _LIBCPP_DEFAULT
    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_CONSTEXPR atomic(_Tp* __d) _NOEXCEPT : __base(__d) {}

    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator=(_Tp* __d) volatile _NOEXCEPT
        {__base::store(__d); return __d;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator=(_Tp* __d) _NOEXCEPT
        {__base::store(__d); return __d;}

    _LIBCPP_INLINE_VISIBILITY
    _Tp* fetch_add(ptrdiff_t __op, memory_order __m = memory_order_seq_cst)
                                                                        volatile _NOEXCEPT
        {return __c11_atomic_fetch_add(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* fetch_add(ptrdiff_t __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_add(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* fetch_sub(ptrdiff_t __op, memory_order __m = memory_order_seq_cst)
                                                                        volatile _NOEXCEPT
        {return __c11_atomic_fetch_sub(&this->__a_, __op, __m);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* fetch_sub(ptrdiff_t __op, memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_fetch_sub(&this->__a_, __op, __m);}

    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator++(int) volatile _NOEXCEPT            {return fetch_add(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator++(int) _NOEXCEPT                     {return fetch_add(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator--(int) volatile _NOEXCEPT            {return fetch_sub(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator--(int) _NOEXCEPT                     {return fetch_sub(1);}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator++() volatile _NOEXCEPT               {return fetch_add(1) + 1;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator++() _NOEXCEPT                        {return fetch_add(1) + 1;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator--() volatile _NOEXCEPT               {return fetch_sub(1) - 1;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator--() _NOEXCEPT                        {return fetch_sub(1) - 1;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator+=(ptrdiff_t __op) volatile _NOEXCEPT {return fetch_add(__op) + __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator+=(ptrdiff_t __op) _NOEXCEPT          {return fetch_add(__op) + __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator-=(ptrdiff_t __op) volatile _NOEXCEPT {return fetch_sub(__op) - __op;}
    _LIBCPP_INLINE_VISIBILITY
    _Tp* operator-=(ptrdiff_t __op) _NOEXCEPT          {return fetch_sub(__op) - __op;}
};

// atomic_is_lock_free

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_is_lock_free(const volatile atomic<_Tp>* __o) _NOEXCEPT
{
    return __o->is_lock_free();
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_is_lock_free(const atomic<_Tp>* __o) _NOEXCEPT
{
    return __o->is_lock_free();
}

// atomic_init

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_init(volatile atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    __c11_atomic_init(&__o->__a_, __d);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_init(atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    __c11_atomic_init(&__o->__a_, __d);
}

// atomic_store

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_store(volatile atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    __o->store(__d);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_store(atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    __o->store(__d);
}

// atomic_store_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_store_explicit(volatile atomic<_Tp>* __o, _Tp __d, memory_order __m) _NOEXCEPT
  _LIBCPP_CHECK_STORE_MEMORY_ORDER(__m)
{
    __o->store(__d, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
atomic_store_explicit(atomic<_Tp>* __o, _Tp __d, memory_order __m) _NOEXCEPT
  _LIBCPP_CHECK_STORE_MEMORY_ORDER(__m)
{
    __o->store(__d, __m);
}

// atomic_load

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_load(const volatile atomic<_Tp>* __o) _NOEXCEPT
{
    return __o->load();
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_load(const atomic<_Tp>* __o) _NOEXCEPT
{
    return __o->load();
}

// atomic_load_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_load_explicit(const volatile atomic<_Tp>* __o, memory_order __m) _NOEXCEPT
  _LIBCPP_CHECK_LOAD_MEMORY_ORDER(__m)
{
    return __o->load(__m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_load_explicit(const atomic<_Tp>* __o, memory_order __m) _NOEXCEPT
  _LIBCPP_CHECK_LOAD_MEMORY_ORDER(__m)
{
    return __o->load(__m);
}

// atomic_exchange

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_exchange(volatile atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    return __o->exchange(__d);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_exchange(atomic<_Tp>* __o, _Tp __d) _NOEXCEPT
{
    return __o->exchange(__d);
}

// atomic_exchange_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_exchange_explicit(volatile atomic<_Tp>* __o, _Tp __d, memory_order __m) _NOEXCEPT
{
    return __o->exchange(__d, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp
atomic_exchange_explicit(atomic<_Tp>* __o, _Tp __d, memory_order __m) _NOEXCEPT
{
    return __o->exchange(__d, __m);
}

// atomic_compare_exchange_weak

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak(volatile atomic<_Tp>* __o, _Tp* __e, _Tp __d) _NOEXCEPT
{
    return __o->compare_exchange_weak(*__e, __d);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak(atomic<_Tp>* __o, _Tp* __e, _Tp __d) _NOEXCEPT
{
    return __o->compare_exchange_weak(*__e, __d);
}

// atomic_compare_exchange_strong

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_strong(volatile atomic<_Tp>* __o, _Tp* __e, _Tp __d) _NOEXCEPT
{
    return __o->compare_exchange_strong(*__e, __d);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_strong(atomic<_Tp>* __o, _Tp* __e, _Tp __d) _NOEXCEPT
{
    return __o->compare_exchange_strong(*__e, __d);
}

// atomic_compare_exchange_weak_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak_explicit(volatile atomic<_Tp>* __o, _Tp* __e,
                                      _Tp __d,
                                      memory_order __s, memory_order __f) _NOEXCEPT
  _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
{
    return __o->compare_exchange_weak(*__e, __d, __s, __f);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_weak_explicit(atomic<_Tp>* __o, _Tp* __e, _Tp __d,
                                      memory_order __s, memory_order __f) _NOEXCEPT
  _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
{
    return __o->compare_exchange_weak(*__e, __d, __s, __f);
}

// atomic_compare_exchange_strong_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_strong_explicit(volatile atomic<_Tp>* __o,
                                        _Tp* __e, _Tp __d,
                                        memory_order __s, memory_order __f) _NOEXCEPT
  _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
{
    return __o->compare_exchange_strong(*__e, __d, __s, __f);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_compare_exchange_strong_explicit(atomic<_Tp>* __o, _Tp* __e,
                                        _Tp __d,
                                        memory_order __s, memory_order __f) _NOEXCEPT
  _LIBCPP_CHECK_EXCHANGE_MEMORY_ORDER(__s, __f)
{
    return __o->compare_exchange_strong(*__e, __d, __s, __f);
}

// atomic_fetch_add

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_add(volatile atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_add(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_add(atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_add(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_add(volatile atomic<_Tp*>* __o, ptrdiff_t __op) _NOEXCEPT
{
    return __o->fetch_add(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_add(atomic<_Tp*>* __o, ptrdiff_t __op) _NOEXCEPT
{
    return __o->fetch_add(__op);
}

// atomic_fetch_add_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_add_explicit(volatile atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_add(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_add_explicit(atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_add(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_add_explicit(volatile atomic<_Tp*>* __o, ptrdiff_t __op,
                          memory_order __m) _NOEXCEPT
{
    return __o->fetch_add(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_add_explicit(atomic<_Tp*>* __o, ptrdiff_t __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_add(__op, __m);
}

// atomic_fetch_sub

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_sub(volatile atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_sub(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_sub(atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_sub(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_sub(volatile atomic<_Tp*>* __o, ptrdiff_t __op) _NOEXCEPT
{
    return __o->fetch_sub(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_sub(atomic<_Tp*>* __o, ptrdiff_t __op) _NOEXCEPT
{
    return __o->fetch_sub(__op);
}

// atomic_fetch_sub_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_sub_explicit(volatile atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_sub(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_sub_explicit(atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_sub(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_sub_explicit(volatile atomic<_Tp*>* __o, ptrdiff_t __op,
                          memory_order __m) _NOEXCEPT
{
    return __o->fetch_sub(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
atomic_fetch_sub_explicit(atomic<_Tp*>* __o, ptrdiff_t __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_sub(__op, __m);
}

// atomic_fetch_and

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_and(volatile atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_and(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_and(atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_and(__op);
}

// atomic_fetch_and_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_and_explicit(volatile atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_and(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_and_explicit(atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_and(__op, __m);
}

// atomic_fetch_or

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_or(volatile atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_or(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_or(atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_or(__op);
}

// atomic_fetch_or_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_or_explicit(volatile atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_or(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_or_explicit(atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_or(__op, __m);
}

// atomic_fetch_xor

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_xor(volatile atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_xor(__op);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_xor(atomic<_Tp>* __o, _Tp __op) _NOEXCEPT
{
    return __o->fetch_xor(__op);
}

// atomic_fetch_xor_explicit

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_xor_explicit(volatile atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_xor(__op, __m);
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    is_integral<_Tp>::value && !is_same<_Tp, bool>::value,
    _Tp
>::type
atomic_fetch_xor_explicit(atomic<_Tp>* __o, _Tp __op, memory_order __m) _NOEXCEPT
{
    return __o->fetch_xor(__op, __m);
}

// flag type and operations

typedef struct atomic_flag
{
    _Atomic(bool) __a_;

    _LIBCPP_INLINE_VISIBILITY
    bool test_and_set(memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {return __c11_atomic_exchange(&__a_, true, __m);}
    _LIBCPP_INLINE_VISIBILITY
    bool test_and_set(memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {return __c11_atomic_exchange(&__a_, true, __m);}
    _LIBCPP_INLINE_VISIBILITY
    void clear(memory_order __m = memory_order_seq_cst) volatile _NOEXCEPT
        {__c11_atomic_store(&__a_, false, __m);}
    _LIBCPP_INLINE_VISIBILITY
    void clear(memory_order __m = memory_order_seq_cst) _NOEXCEPT
        {__c11_atomic_store(&__a_, false, __m);}

    _LIBCPP_INLINE_VISIBILITY
#ifndef _LIBCPP_CXX03_LANG
    atomic_flag() _NOEXCEPT = default;
#else
    atomic_flag() _NOEXCEPT : __a_() {}
#endif // _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    atomic_flag(bool __b) _NOEXCEPT : __a_(__b) {} // EXTENSION

#ifndef _LIBCPP_CXX03_LANG
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) volatile = delete;
#else
private:
    atomic_flag(const atomic_flag&);
    atomic_flag& operator=(const atomic_flag&);
    atomic_flag& operator=(const atomic_flag&) volatile;
#endif
} atomic_flag;

inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_flag_test_and_set(volatile atomic_flag* __o) _NOEXCEPT
{
    return __o->test_and_set();
}

inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_flag_test_and_set(atomic_flag* __o) _NOEXCEPT
{
    return __o->test_and_set();
}

inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_flag_test_and_set_explicit(volatile atomic_flag* __o, memory_order __m) _NOEXCEPT
{
    return __o->test_and_set(__m);
}

inline _LIBCPP_INLINE_VISIBILITY
bool
atomic_flag_test_and_set_explicit(atomic_flag* __o, memory_order __m) _NOEXCEPT
{
    return __o->test_and_set(__m);
}

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_flag_clear(volatile atomic_flag* __o) _NOEXCEPT
{
    __o->clear();
}

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_flag_clear(atomic_flag* __o) _NOEXCEPT
{
    __o->clear();
}

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_flag_clear_explicit(volatile atomic_flag* __o, memory_order __m) _NOEXCEPT
{
    __o->clear(__m);
}

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_flag_clear_explicit(atomic_flag* __o, memory_order __m) _NOEXCEPT
{
    __o->clear(__m);
}

// fences

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_thread_fence(memory_order __m) _NOEXCEPT
{
    __c11_atomic_thread_fence(__m);
}

inline _LIBCPP_INLINE_VISIBILITY
void
atomic_signal_fence(memory_order __m) _NOEXCEPT
{
    __c11_atomic_signal_fence(__m);
}

// Atomics for standard typedef types

typedef atomic<bool>               atomic_bool;
typedef atomic<char>               atomic_char;
typedef atomic<signed char>        atomic_schar;
typedef atomic<unsigned char>      atomic_uchar;
typedef atomic<short>              atomic_short;
typedef atomic<unsigned short>     atomic_ushort;
typedef atomic<int>                atomic_int;
typedef atomic<unsigned int>       atomic_uint;
typedef atomic<long>               atomic_long;
typedef atomic<unsigned long>      atomic_ulong;
typedef atomic<long long>          atomic_llong;
typedef atomic<unsigned long long> atomic_ullong;
typedef atomic<char16_t>           atomic_char16_t;
typedef atomic<char32_t>           atomic_char32_t;
typedef atomic<wchar_t>            atomic_wchar_t;

typedef atomic<int_least8_t>   atomic_int_least8_t;
typedef atomic<uint_least8_t>  atomic_uint_least8_t;
typedef atomic<int_least16_t>  atomic_int_least16_t;
typedef atomic<uint_least16_t> atomic_uint_least16_t;
typedef atomic<int_least32_t>  atomic_int_least32_t;
typedef atomic<uint_least32_t> atomic_uint_least32_t;
typedef atomic<int_least64_t>  atomic_int_least64_t;
typedef atomic<uint_least64_t> atomic_uint_least64_t;

typedef atomic<int_fast8_t>   atomic_int_fast8_t;
typedef atomic<uint_fast8_t>  atomic_uint_fast8_t;
typedef atomic<int_fast16_t>  atomic_int_fast16_t;
typedef atomic<uint_fast16_t> atomic_uint_fast16_t;
typedef atomic<int_fast32_t>  atomic_int_fast32_t;
typedef atomic<uint_fast32_t> atomic_uint_fast32_t;
typedef atomic<int_fast64_t>  atomic_int_fast64_t;
typedef atomic<uint_fast64_t> atomic_uint_fast64_t;

typedef atomic< int8_t>  atomic_int8_t;
typedef atomic<uint8_t>  atomic_uint8_t;
typedef atomic< int16_t> atomic_int16_t;
typedef atomic<uint16_t> atomic_uint16_t;
typedef atomic< int32_t> atomic_int32_t;
typedef atomic<uint32_t> atomic_uint32_t;
typedef atomic< int64_t> atomic_int64_t;
typedef atomic<uint64_t> atomic_uint64_t;

typedef atomic<intptr_t>  atomic_intptr_t;
typedef atomic<uintptr_t> atomic_uintptr_t;
typedef atomic<size_t>    atomic_size_t;
typedef atomic<ptrdiff_t> atomic_ptrdiff_t;
typedef atomic<intmax_t>  atomic_intmax_t;
typedef atomic<uintmax_t> atomic_uintmax_t;

#define ATOMIC_FLAG_INIT {false}
#define ATOMIC_VAR_INIT(__v) {__v}

_LIBCPP_END_NAMESPACE_STD

#endif  // _LIBCPP_ATOMIC
