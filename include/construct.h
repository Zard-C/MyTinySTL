// -- construct.h 与对象空间分配与回收，对象构建与析构有关的辅助函数 undone  
#ifndef CONSTRUCT_H_ 
#define CONSTRUCT_H_ 
#include <new> 
#include <cstdlib>
#include <iostream> 
#include "util.h"

namespace mystl
{

template<typename T> 
inline T* _allocate(ptrdiff_t size, T*)
{
    std::set_new_handler(0);
    T* tmp = static_cast<T*> (::operator new(static_cast<size_t>(size * sizeof(T))));
    if(tmp == 0) {std::cerr<<"out of memory"<< std::endl; exit(-1);}
    return tmp; 
}


template<typename T> 
inline void _deallocate(T* buffer)
{
    ::operator delete(buffer);
}

template <typename T>
void _construct(T* ptr)
{
  ::new ((void*)ptr) T();
}


// helper functions  const T& 
template<typename T1, typename T2> 
inline void _construct(T1 *ptr, const T2& value)
{
    ::new(static_cast<void*>(ptr)) T1(value);
}


//  移动构造版本 T&&     
template <typename T1, class... Args>
void _construct(T1* ptr, Args&&... args)
{
  ::new ((void*)ptr) T1(mystl::forward<Args>(args)...); // 调用移动构造版本
}


template<typename T>  
inline void _destroy(T* ptr)
{
    ptr->~T(); 
}

// 单个对象销毁
// overload 
template<typename T> 
void destroy_one(T*, std::true_type)
{

}

// overload
template<typename T> 
void destroy_one(T* ptr, std::false_type)
{
    if(ptr)
    {
        ptr->~T(); 
    }
}

// 多个对象销毁
// overload
template<typename ForwardIterator> 
void destroy_cat(ForwardIterator, ForwardIterator, std::true_type)
{

}

// overload 
template<typename ForwardIterator> 
void destroy_cat(ForwardIterator first, ForwardIterator last, std::false_type)
{
    for(; first != last; ++first)
    {
        _destroy(&(*first)); // 通过迭代器获得实际上要释放的地址
    }

}

// 单个元素的释放
// std::is_trivially_destructible
template <typename T> 
void destroy(T* ptr)
{
    destroy_one(ptr, std::is_trivially_destructible<T>{}); 
}
 
// 多个元素的释放
template <typename ForwardIterator> 
void destroy(ForwardIterator first, ForwardIterator last)
{
    destroy_cat(first, last, std::is_trivially_destructible<
        typename std::iterator_traits<ForwardIterator>::value_type>{}
    ); 
    // iterator traits mechanism 
}

} // end of namespace mystl 
#endif // !CONSTRUCT_H_ 
