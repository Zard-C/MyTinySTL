// -- functional.h mystl 中的仿函数以及哈希函数
#ifndef FUNCTIONAL_H_ 
#define FUNCTIONAL_H_ 

#include <cstddef> 
#include <functional> 

namespace mystl 
{
// 一元函数参数型别和返回值型别
template <typename Arg, typename Result> 
struct unarg_function 
{
    typedef Arg         argument_type ; 
    typedef Result      result_type; 
}; 

// 二元函数参数型别和返回值型别 
template <typename Arg1, typename Arg2, typename Result> 
struct binary_function
{
    typedef Arg1        first_argument_type; 
    typedef Arg2        second_argument_type; 
    typedef Result      result_type; 
};

// 仿函数: 加法
template <typename T> 
struct plus: public std::binary_function<T, T, T> 
{
    T operator()(const T& x, const T& y) const  {return x + y; }
}; 

// 仿函数: 减 
template <typename T> 
struct minus: public std::binary_function<T, T, T> 
{
    T operator()(const T& x, const T& y) const  {return x - y; }
}; 

// 仿函数: 乘法
template <typename T> 
struct multiplies: public std::binary_function<T, T, T> 
{
    T operator()(const T& x, const T& y) const  {return x * y; }
}; 

// 仿函数: 除法 
template <typename T> 
struct divides:public std::binary_function<T, T, T> 
{
    T operator()(const T& x, const T& y) const  {return x / y; }
}; 

// 仿函数: 模取
template <typename T> 
struct modulus:public std::binary_function<T, T, T> 
{
    T operator()(const T& x, const T& y) const {return x % y; }
}; 

// 一元运算
// 仿函数：否定
template <typename T> 
struct negate: public std::unary_function<T, T>
{
    T operator()(const T& x) const {return -x; }
};

// identity for plus 
template <typename T> 
T identity_element(plus<T>) { return T(0);} 

// identity for multiply 
template <typename T> 
T identity_element(multiplies<T>) { return T(1);} 

// equal_to 
template <typename T> 
struct equal_to: public std::binary_function<T, T, bool> 
{
    bool operator()(const T&x, const T& y) const {return x == y; }
}; 

// not_equal_to 
template <typename T> 
struct not_equal_to: public std::binary_function<T, T, bool> 
{
    bool operator()(const T&x, const T& y) const {return x != y; }
}; 

// greater  
template <typename T> 
struct greater: public std::binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {return x > y; }
}; 

// less 
template <typename T> 
struct less: public std::binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {return x < y; }
}; 

// greater_equal 
template <typename T> 
struct greater_equal: public std::binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {return x >=y;}
}; 

// less_equal 
template <typename T> 
struct less_equal: public std::binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {return x <=y;}
}; 

// logic && 
template <typename T> 
struct logical_and: public std::binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {return x && y;}
}; 

// logic || 
template <typename T> 
struct logical_or: public std::binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {return x || y;}
}; 

// logic !
template <typename T> 
struct logical_not: public std::unary_function<T, bool> 
{
    bool operator()(const T& x) const {return !x; }
}; 

// identity return self type 
template <typename T> 
struct identity: public unarg_function<T, bool> 
{
    const T& operator()(const T& x) {return x; }
}; 

// selectfirst 
template <typename Pair>
struct selectfirst: public unarg_function<Pair, typename Pair::first_type>
{
    const typename Pair::first_type& operator()(const Pair & x) const 
    {
        return x.first; 
    }
} ; 

// selectsecond 
template <typename Pair>
struct selectsecond: public unarg_function<Pair, typename Pair::second_type>
{
    const typename Pair::second_type& operator()(const Pair & x) const 
    {
        return x.second; 
    }
}; 

// 点(first, second) 投影在 x 轴: first, y 轴: second 
// projectfirst： 投影函数, 返回第一个参数
template <typename Arg1, typename Arg2> 
struct projectfirst: public binary_function<Arg1, Arg2, Arg1> 
{
    Arg1 operator()(const Arg1& x, const Arg2& ) const {return x; }
}; 

// projectsecond： 投影函数, 返回第二个参数
template <typename Arg1, typename Arg2> 
struct projectsecond: public binary_function<Arg1, Arg2, Arg2> 
{
    Arg2 operator()(const Arg1& , const Arg2& y) const {return y; }
}; 

// hash 这块看一下STL 源码实现
# if 0 
// 哈希函数对象
// 对于大部分类型，hash function 做不了什么 
template <typename Key> 
struct hash {} ; 

// pointer 偏特化版本 
template <typename T> 
struct hash<T*> 
{
   size_t operator()(T* p ) const noexcept
   {
       return reinterpret_cast<size_t>(p); 
   }
};

// 对于整形类型，返回原值即可
template<>
struct hash<bool> 
{
    size_t operator()(bool val) const noexcept
    {
        return reinterpret_cast<size_t>(val); 
    }
}; 
#endif 


}   // end of namespace mystl
#endif // !FUNCTIONAL_H_ 
