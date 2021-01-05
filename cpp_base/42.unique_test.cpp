#include <type_traits>
#include <utility>
#include <tuple>
using namespace std;
template<typename _Tp>//删除单个元素
struct default_delete
{
    /// Default constructor
    default_delete(){}

    /** @brief Converting constructor.
     *
     * Allows conversion from a deleter for arrays of another type, @p _Up,
     * only if @p _Up* is convertible to @p _Tp*.
     */
    template<typename _Up>
    default_delete(const default_delete<_Up>&) { }

    /// Calls @c delete @p __ptr
    void
    operator()(_Tp* __ptr) const
    {
        static_assert(!is_void<_Tp>::value,
                    "can't delete pointer to incomplete type");
        static_assert(sizeof(_Tp)>0,
                    "can't delete pointer to incomplete type");
        delete __ptr;
    }
};


template <class _Tp> 
struct remove_reference        
{
    typedef _Tp type;
};

template<typename _Tp,typename _Dp = default_delete<_Tp> >
class _Pointer
{
    template<typename _Up>
    static typename _Up::pointer __test(typename _Up::pointer*);

    template<typename _Up>
    static _Tp* __test(...);//返回_Tp*的__test函数
    typedef typename remove_reference<_Dp>::type _Del;//对于模板类型，需要加typename
public:
    typedef decltype(__test<_Del>(0)) type;//__test第一个类型
};

int main()
{
    return 0;
}