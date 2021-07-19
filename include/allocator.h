// -- allocator.h 空间分配器实现 tested 
#ifndef ALLOCATOR_HPP_  
#include <cstddef>
#include <climits> 
#include "construct.h"

namespace mystl
{

template<typename T> 
class allocator
{
public: 
    // type traits 
    typedef T           allocator_type;
    typedef T           value_type; 
    typedef T*          pointer; 
    typedef const T*    const_pointer; 
    typedef T&          reference; 
    typedef const T&    const_reference; 
    typedef size_t      size_type; 
    typedef ptrdiff_t   difference_type; 
    
    static pointer allocate();
    static pointer allocate(size_type n); 

    static void deallocate(pointer ptr); 
    static void deallocate(pointer ptr, size_type n); 

    static void construct(pointer ptr); 
    static void construct(pointer ptr, const_reference& value); 
    static void construct(pointer ptr, T&& value); 

    template <typename...Args>
    static void construct(pointer ptr, Args&& ...args); 

    static void destroy(pointer ptr); 
    static void destroy(pointer first, pointer last); 
};

template <typename T> 
T* allocator<T>::allocate()
{
    return static_cast<T*>(::operator new(sizeof(T))); 
}

template<typename T>   
T* allocator<T>::allocate(size_type n)
{
    if(0 == n ) return nullptr; 
    return static_cast<T*>(::operator new(n * sizeof(T))); 
}

template <typename T>
void allocator<T>::deallocate(T* ptr)
{
    if(nullptr == ptr) return; 
    ::operator delete(ptr); 
}

template <typename T> 
void allocator<T>::deallocate(T* ptr, size_type)
{
    if(ptr == nullptr) return; 
    ::operator delete(ptr);
}

template <typename T> 
void allocator<T>::construct(T* ptr, const T& value)
{
    mystl::_construct(ptr, value); 
}

template <typename T> 
void allocator<T>::construct(T* ptr, T&& value)
{
    mystl::_construct(ptr, mystl::move(value));
}

template <typename T> 
template <typename ...Args>   
void allocator<T>::construct(T* ptr, Args&& ...args)
{
    mystl::_construct(ptr, mystl::forward<Args>(args)...); 
}

template <typename T>
void allocator<T>::destroy(T* ptr)
{
    mystl::destroy(ptr); 
}

template <typename T> 
void allocator<T>:: destroy(T* first, T* last)
{
    mystl::destroy(first, last); 
}

} // end of namespace 
#endif // !ALLOCATOR_HPP_  


