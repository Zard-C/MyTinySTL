// -- type_traits.h 类型萃取 tested 
#ifndef TYPE_TRAITS_H_ 
#define TYPE_TRAITS_H_ 
#include <type_traits> 

namespace mystl
{
// 用于提供编译期常量
// m_integral_constant is designed to provide compile-time constants as types.
template <typename T, T v> 
struct m_integral_constant
{
    static constexpr T value = v; 
    typedef T value_type; 
    typedef m_integral_constant<T, v> type; 
    constexpr operator T(){return v;}       // c++17 
};

// bool_constant 
template <bool b> 
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true> m_true_type; 
typedef m_bool_constant<false> m_false_type; 

// type traits
template<typename T1, typename T2> 
struct pair; 

// 继承typedef 泛型模版
template<typename T> 
struct is_pair: mystl::m_false_type {} ; 

// 继承typedef 模版特化
template <typename T1, typename T2> 
struct is_pair<mystl::pair<T1, T2>>: m_true_type {}; 


} // end of mystl 

#endif // !TYPE_TRAITS_H_ 
