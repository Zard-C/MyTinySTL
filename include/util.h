// --util.h  not tested 
// 通用工具集: 函数 move, forward, swap 
// 模版类 pair 
#ifndef UTIL_H_ 
#define UTIL_H_ 

#include <cstddef> 
#include <type_traits> 
#include <iostream> 

namespace mystl
{
// move 移动语义
template <typename T> 
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&& >(arg); 
}

// forward 完美转发
template <typename T>  
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg); 
}

template<typename T> 
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&> (arg);
}

// swap 
template <typename Tp> 
void swap(Tp& lhs, Tp& rhs)
{
    // **使用移动语义**
    auto tmp(mystl::move(lhs)); 
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp); 
}

// range swap iterator 
template <typename ForwardIter1, typename ForwardIter2> 
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for(; first1 != last1; ++first1, ++first2)
    {
        mystl::swap(*first1, *first2);
    }
    return first2; 
}

// range swap Tp array swap 
template<typename Tp, size_t N> 
void swap(Tp(&a)[N], Tp(&b)[N])
{
    mystl::swap_range(a, a + N, b); 
}

// pair 目前并不能与STL完全兼容
// pair.first, pair.second; 
template<typename T1, typename T2> 
struct pair
{
    typedef T1 first_type; 
    typedef T2 second_type; 

    first_type      first; 
    second_type     second; 

   
    
    
    // default constuctiable 判断Other1 Other2 类型是否可以默认构造
    template<typename Other1 = T1, typename Other2 = T2, 
    typename = typename std::enable_if<
    std::is_default_constructible<Other1>::value &&
    std::is_default_constructible<Other2>::value, void>::type> 
    constexpr pair(): first(), second() {} 
    
    // implicit constructible for this type  判断U1，U2类型是否能向T1
    // T2  类型进行隐式转换 判断U1 U2是否为可拷贝构造类型
    template<typename U1 = T1 , typename U2 = T2, 
    typename std::enable_if< 
    std::is_copy_constructible<U1>::value && 
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value && 
    std::is_convertible<const U2&, T2>::value, int>::type =0> 
    constexpr pair(const T1& a, const T2& b): first(a), second(b) {} 


    // explicit constructible for this type 
    template<class Other1, class Other2, 
     typename std::enable_if< 
     std::is_constructible<T1, Other1>::value && 
     std::is_constructible<T2, Other2>::value && 
     (!std::is_convertible<Other1, T1>::value || 
      !std::is_convertible<Other2, T1>::value), int>::type = 0 >
    explicit constexpr pair(pair<Other1, Other2> && other):
    first(mystl::forward<Other1>(other.first)), 
    second(mystl::forward<Other2>(other.second))
    {}

    // implicit construcible for pair<U1, U2> 
    template<typename U1 = T1 , typename U2 = T2, 
    typename std::enable_if< 
    std::is_copy_constructible<U1>::value && 
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value && 
    std::is_convertible<const U2&, T2>::value, int>::type =0> 
    constexpr pair(const pair<U1, U2> & other): first(other.first), second(other.second) 
    {}


    // default ctors
    pair(const pair& rhs) = default; 
    pair(pair&& rhs) = default; 
    ~pair() = default; 
    

    // copy assign 
    pair& operator=(const pair &rhs)
    {
        if(this != &rhs)
        {
            first = rhs.first; 
            second = rhs.second; 
        }
        return *this; 
    }

    // move assign 
    pair& operator=(pair&& rhs)
    {
        if(this != &rhs)
        {
            first = mystl::move(rhs.first); 
            second = mystl::move(rhs.second); 
        }
        return *this; 
    }

    // copy assign: other pair 
    template <typename Other1, typename Other2> 
    pair& operator=(pair<Other1, Other2>& other)
    {
        first = other.first;
        second = other.second;
        return *this; 
    }

    // move assign: other pair 
    template <typename Other1, typename Other2>
    pair& operator=(pair<Other1, Other2>&& other)
    {
        first = mystl::forward<Other1>(other.first);
        second = mystl::forward<Other2>(other.second); 
        return *this; 
    } 


    void swap(pair& other)
    {
        if(this != &other) // bug fixed &
        {
            mystl::swap(first, other.first); 
            mystl::swap(second, other.second); 
        }
    }

}; 

// 操作符重载 实现 < ==即可实现剩下四种运算符重载

// equal 
template <typename T1, typename T2> 
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second; 
}

// not equal 
template<typename T1, typename T2> 
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs == rhs); 
}

// less 
template<typename T1, typename T2> 
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first < rhs.first ||(lhs.first == rhs.first && lhs.second < rhs.second); 
}

// greater equal 
template<typename T1, typename T2> 
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs < rhs); 
}

// greater
template <typename T1, typename T2> 
bool operator > (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return rhs < lhs; 
}

// less equal 
template<typename T1, typename T2> 
bool operator <= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(rhs < lhs); 
}

// overload mystl::swap
template<typename T1, typename T2> 
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
{
    lhs.swap(rhs); 
}

// mystl::make_pair 
template<typename T1, typename T2> 
pair<T1, T2> make_pair(T1&& first, T2&& second)
{
    return pair<T1, T2>(std::forward<T1>(first), std::forward<T2>(second)); 
}


} // end of mystl 

#endif // !UTIL_H_ 