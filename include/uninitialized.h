// -- uninitialized.h 用于未初始化的空间构造元素
#ifndef UNINITIALIZED_H_ 
#define UNINITIALIZED_H_ 

#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h" 
#include <memory> 

namespace mystl
{

// uninitialized copy -> is_trivially_copy_assignable == true
template<typename InputIter, typename ForwardIter> 
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return std::copy(first, last, result);
}

// uninitialized copy -> is_trivially_copy_assignable
template<typename InputIter, typename ForwardIter> 
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    auto cur = result; 
    try
    {
        for(; first !=last; ++first, ++cur)
        {
            mystl::_construct(&*cur, *first);  // 迭代器重载 *& 运算符
        }
    }
    catch(...)
    {
        // 出现异常，将之前构造的元素析构。
        for(; result != cur; ++result)
        {
            mystl::destroy(&*result); 
        }
    }
    return cur; 
}

// 通过判断is_trivially_copy_assignable选择调用哪个函数
template<typename InputIter, typename ForwardIter> 
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    return mystl::unchecked_uninit_copy(first, last, result , 
        std::is_trivially_copy_assignable< 
        typename iterator_traits<ForwardIter>::value_type>{}
    ); 
}

// uninitialized_copy_n  
// 区间复制[0,n)并返回复制结束位置 is_trivially_copy_assignable -> true 
template <typename InputIter, typename Size, typename ForwardIter>
ForwardIter
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::true_type)
{
    return std::copy(first, n, result); 
}

// is_trivially_copy_assignable->false 
template <typename InputIter, typename Size, typename ForwardIter>
ForwardIter
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
    auto cur = result; 
    try
    {
        for(; n > 0; --n, ++cur, ++first)
        {
            mystl::_construct(&*cur, *first); 
        }
    }
    catch(...)
    {
        for(; result != cur; ++result)
        {
            mystl::destroy(&*result); 
        }
    }
    return cur; 
}

// 区间元素拷贝
template <typename InputIter, typename Size, typename ForwardIter>
ForwardIter
uninitialized_copy_n(InputIter first, Size n, ForwardIter result)
{
    return unchecked_uninit_copy_n(first, n, result, 
    std::is_trivially_copy_assignable<
    typename iterator_traits<InputIter>::value_type>{}
    ); 
}

// uninitailized_fill is_trivially_copy_assignable->true 
// [first, last) 填充元素值
template <typename ForwardIter, typename T> 
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type)
{
    std::fill(first, last, value); 
}

// is_trivially_copy_assignable -> false 
template <typename ForwardIter, typename T> 
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type)
{
    auto curr = first; 
    try
    {
        for(; curr != last; ++curr)
        {
            mystl::_construct(&*curr, value); 
        }
    }
    catch(...) 
    {
        for(; first != curr; ++first)
        {
            mystl::destroy(&*first); 
        }
    }
}

// 
template <typename ForwardIter, typename T> 
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value)
{
    mystl::unchecked_uninit_fill(first, last, value, 
        std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{} 
    ); 
}


// unintialized_fill_n 
// 从first 开始填充n个元素, 返回填充结束位置    is_trivially_copy_assignable -> true 
template <typename ForwardIter, typename Size, typename T> 
ForwardIter 
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type)
{
    return std::fill_n(first, n, value); 
}

// is_trivially_copy_assignable -> false; 
template <typename ForwardIter, typename Size, typename T> 
ForwardIter 
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type)
{
    auto curr = first; 
    try
    {
        for(; n>0; --n, ++curr)
        {
            mystl::_construct(&*curr, value); 
        }
    }
    catch(...)
    {
        for(; first != curr; ++first)
        {
            mystl::destroy(&*first); 
        }
    }
    return curr; 
}

// uninitialized_fill_n: 
template <typename ForwardIter, typename Size, typename T> 
ForwardIter 
uninitialized_fill_n(ForwardIter first, Size n, const T&value)
{
    return unchecked_uninit_fill_n(first, n, value, 
        std::is_trivially_copy_assignable<typename  iterator_traits<ForwardIter>::value_type>{}
        ); 
}

// uninitialized_move 转移所有权 is_trivially_move_assignable-> true 
// [first, last) 上的内容移动到 result 为起始位置的空间，返回移动结束的位置
template <typename InputIter, class ForwardIter> 
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return std::move(first, last, result); 
}

// is_trivially_move_assignable->false 
template <typename InputIter, class ForwardIter> 
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    ForwardIter curr = result; 
    try
    {
        for(; first != last; ++first, ++curr)
        {
            _construct(&*curr, mystl::move(*first)); 
        }
    }
    catch(...)
    {
        mystl::destroy(result, curr);
    }
    return curr; 
}

// 区间move: 所有权的转移   
template <typename InputIter, class ForwardIter> 
ForwardIter 
uninitialized_move(InputIter first, InputIter last, ForwardIter result)
{
    return mystl::unchecked_uninit_move(first, last, result, 
        std::is_trivially_move_assignable<typename iterator_traits<InputIter>::value_type>{} 
        ); 
}

// uninitialized_move_n is_trivially_move_assignable-> true 
// [first, last) 上资源所有权转移到 result 为起始处空间，返回结束位置
template <typename InputIter, typename Size, typename ForwardIter> 
ForwardIter 
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
{
    return mystl::move(first, first + n, result); 
}

// is_trivially_move_assignable -> false 
template <typename InputIter, typename Size, typename ForwardIter> 
ForwardIter 
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
    ForwardIter curr = result; 
    try
    {
        for(; n > 0; --n, ++curr, ++first)
        {
            mystl::_construct(&*curr, mystl::move(*first)); // 转移所有权
        }
    }
    catch(...)
    {
       for(; result != curr; ++result)
       {
           mystl::destroy(&*result); 
       }
    }
    return curr; 
}

// unintialized_move_n
template <typename InputIter, typename Size, typename ForwardIter> 
ForwardIter 
uninitialized_move_n(InputIter first, Size n, ForwardIter result)
{
    return unchecked_uninit_move_n(first, n, result,    
        std::is_trivially_move_assignable<typename iterator_traits<InputIter>::value_type>{}

    ); 
}
} // end of mystl 
#endif // !UNINITIALIZED_H_ 