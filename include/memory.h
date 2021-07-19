// -- memory.h undone
// 负责更高级的动态内存管理
// 基本函数 空间配置器 未初始化的空间管理，模版类 auto_ptr 
#ifndef MEMORY_H_
#define MEMORY_H_ 
#include "construct.h"
#include "allocator.h"
#include "uninitialized.h"


#include <cstddef> 
#include <cstdlib>
#include <climits> 
#include <type_traits> 

namespace mystl
{

// 获取对象地址
template <typename Tp> 
constexpr Tp* address_of(Tp& value) noexcept 
{
    return &value; 
}

// temp use std::pair, should use mystl::pair 


// 获取/释放临时缓冲区 辅助函数
template <typename T>  
std::pair<T*, ptrdiff_t> _get_buffer(ptrdiff_t len, T*)
{
    if(len > static_cast<ptrdiff_t>(INT_MAX/sizeof(T)))
        len = INT_MAX/sizeof(T); 
    while(len > 0)
    {
        T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len)*sizeof(T)));
        if(tmp)
            return std::pair<T*, ptrdiff_t>(tmp, len); 
        len /=2; // 申请失败时将len减半
    }
    return std::pair<T*, ptrdiff_t>(nullptr, 0);
};

// Allocates uninitialized contiguous storage, 
// which should be sufficient to store up to count adjacent objects of type T. 
// The request is non-binding and the implementation may allocate less or 
// more than necessary to store count adjacent objects.
template<typename T> 
std::pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len)
{
    // 通过T* 传递类型信息
    return _get_buffer(len, static_cast<T*>(0)); 
}

// overloads
template<typename T> 
std::pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*)
{
    // 通过T* 传递类型信息
    return _get_buffer(len, static_cast<T*>(0)); 
}

// 释放临时缓冲区
template<typename T>  
void release_temporary_buffer(T* ptr)
{
    free(ptr); 
}


// 
template<typename ForwardIterator, typename T>  
class temporary_buffer
{
private: 
    ptrdiff_t   original_len;         // 申请容量
    ptrdiff_t   len;                  // 实际申请到的容量
    T*          buffer;

public: 
    // ctor 
    temporary_buffer(ForwardIterator first, ForwardIterator last); 
    ~temporary_buffer()
    {
        mystl::destroy(buffer, buffer + len);
        free(buffer); 
    }


public: 
    ptrdiff_t size()                const noexcept {return len; }
    ptrdiff_t requested_size()      const noexcept{return original_len; }
    T* begin()                      noexcept {return buffer; }
    T* end()                        noexcept {return buffer + len; }

private: 
    void allocate_buffer();
    void initialize_buffer(const T&, std::true_type){}
    void initialize_buffer(const T& value, std::false_type)
    {
        std::uninitialized_fill_n(buffer, len, value); 
    }

    // delete  copy ctor and  copy assgin  
    temporary_buffer(const temporary_buffer& ) =delete; 
    void operator=(const temporary_buffer&) = delete; 
}; 

// ctor 
template<typename ForwardIterator, typename T>  
temporary_buffer<ForwardIterator, T>::
temporary_buffer(ForwardIterator first, ForwardIterator last)
{
    try
    {
        len = std::distance(first, last); 
        allocate_buffer(); 
        if(len > 0)
        {
            initialize_buffer(*first, std::is_trivially_default_constructible<T>()); 
        }
    }
    catch(...)
    {
        free(buffer);
        buffer = nullptr; 
        len = 0; 
    }
}

// allocate_buffer;
template<typename ForwardIterator, typename T>  
void temporary_buffer<ForwardIterator, T>::
allocate_buffer()
{
    original_len = len; 
    if(len > static_cast<ptrdiff_t>(INT_MAX/sizeof(T)))
        len = INT_MAX/sizeof(T);
    while(len > 0)
    {
        buffer = static_cast<T*>(malloc(len * sizeof(T)));
        if(buffer) break; 
        len /= 2; // 空间不足时尝试折半
    }

}

// auto_ptr template
// 
template <typename T> 
class auto_ptr
{
public: 
    typedef T   elem_type; 

private:   
    T* ptr; 

public:     
    // ctor 
    explicit auto_ptr(T* p= nullptr): ptr(p) {} 
    auto_ptr(auto_ptr& rhs):ptr(rhs.release()){}

    template<typename U>   
    auto_ptr(auto_ptr<U>& rhs):ptr(rhs.release()){} 

    // copy assign 
    auto_ptr& operator=(auto_ptr& rhs)
    {
        if(this !=&rhs)
        {
            delete ptr; 
            ptr = rhs.release();
        }
        return *this; 
    }

    template <typename U>   
    auto_ptr& operator=(auto_ptr<U>& rhs)
    {
        if(this->get() != rhs.get())
        {
            delete ptr; 
            ptr = rhs.release(); 
        }
        return *this; 
    }

    // dtor 
    ~auto_ptr(){delete ptr;}

    // overloads 
    T& operator*() const {return *ptr; }
    T& operator->() const {return ptr; }

    // aux methods 
    T* get() const {return ptr; }
    
    // release resources 
    T* release()
    {
        T* tmp = ptr; 
        ptr = nullptr; 
        return tmp; 
    }

    // reset ptr; 
    void reset(T* p = nullptr)
    {
        if(ptr != p)
        {
            delete ptr; // release resource a
            ptr = p; 
        }
    }

};


} // endof namespace mystl 



#endif // !MEMORY_H_