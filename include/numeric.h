// -- numeric.h 数值算法
#ifndef NUMERIC_H_ 
#define  NUMERIC_H_ 

#include "iterator.h" 

namespace mystl 
{

// accumulate: 以init 为初值对[first, last) 区间累计求和
template<typename InputIter, typename T> 
T accumulate(InputIter first, InputIter last, T init)
{
    for(; first != last; ++first)
    {
        init += *first; 
    }
    return std::move(init);    // move ?  
}

// accumulate: 以init 为初值对[first, last) 区间自定进行自定义操作
template<typename InputIter, typename T, typename BinaryOp> 
T accumulate(InputIter first, InputIter last, T init, BinaryOp binary_op)
{
    for(; first != last; ++first)
    {
        init = binary_op(init, *first); 
    }
    return init;    // move ?  
}

// adjacent_difference 
// 计算相邻元素的差值，结果保存到以 result 为起始的区间上
template<typename InputIter, typename OutputIter> 
OutputIter 
adjacent_difference(InputIter first, InputIter last, OutputIter result)
{
    if(first == last ) return result; 
    *result = *first;
    auto value = *first; 

    while(++first != last)
    {
        auto temp = *first; 
        *++result = temp - value;
        value = temp;  
    }
    return ++result; 
}

// adjacent_difference: 自定义操作
template<typename InputIter, typename OutputIter, typename BinaryOp> 
OutputIter 
adjacent_difference(InputIter first, InputIter last, OutputIter result, 
                    BinaryOp binary_op) 
{
    if(first == last ) return result; 
    *result = *first;
    auto value = *first; 

    while(++first != last)
    {
        auto temp = *first; 
        *++result = binary_op(temp, value); 
        value = temp;  
    }
    return ++result; 
}

// inner_product: 以init为初值, 计算两个向量的内积
template <typename InputIter1, typename InputIter2, typename T> 
T inner_product(InputIter1 first1, InputIter1 last1, 
                InputIter2 first2,
                T init)
{
    for(; first1 != last1; ++first1, ++first2)
    {
        init = init + (*first1 * *first2);
    }
    return init; 
}

// inner_product: 计算两个向量的广义内积，使用自定义运算符 
template <typename InputIter1, typename InputIter2, typename T, typename BinaryOpPlus, typename BinaryOpMult> 
T inner_product(InputIter1 first1, InputIter1 last1, 
                InputIter2 first2, 
                T init, 
                BinaryOpPlus binary_op_plus, 
                BinaryOpMult binary_op_mult) 
{
    for(; first1 != last1; ++first1, ++first2)
    {
        init = binary_op_plus(init, binary_op_mult(*first1, *first2)); 
    }
    return init; 
}

// iota: 以value为初始值填充[first, last) 
template <typename ForwardIter, typename T> 
constexpr void iota(ForwardIter first, ForwardIter last, T value)
{
    for(; first != last; ++first, ++value)
    {
        *first = value; 
    }
}
 
// partial_sum: 求[first, last) 前缀和，结果保存在result 为起始的区间上
template <typename InputIter, typename OutputIter> 
OutputIter 
partial_sum(InputIter first, InputIter last, OutputIter result) 
{
    if(first == last ) return result; 
    *result = *first; 
    auto value = *first; 

    while(++first != last) 
    {
        value = value + *first; 
        *++result = value; 
    }
    return ++result; 
}

// partial_sum: 自定义操作
template <typename InputIter, typename OutputIter, typename BinaryOp> 
OutputIter 
partial_sum(InputIter first, InputIter last, OutputIter result,
            BinaryOp binary_op) 
{
    if(first == last ) return result; 
    *result = *first; 
    auto value = *first; 

    while(++first != last) 
    {
        value = binary_op(value, *first);  
        *++result = value; 
    }
    return ++result; 
}


} // end of namespace stl 
#endif // !NUMERIC_H_ 