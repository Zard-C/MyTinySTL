// -- vector.h 模版类vector 
#ifndef VECTOR_H_ 
#define VECTOR_H_ 

// notes:   
// 异常保证
// 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证
// * emplace 
// * emplace_back
// * push_back
// 当std::is_nothrow_move_assignable<T>::value == true_type时 以下函数
// 也满足强异常安全保证
// * reserve 
// * resize
// * insert 

#include <algorithm> 
#include <initializer_list> 
#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{

template <typename T>   
class vector
{
    static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned in mystl"); 
public: 
    // traits 
    typedef mystl::allocator<T>                     allocator_type; 
    typedef mystl::allocator<T>                     data_allocator; 

    typedef typename allocator_type::value_type         value_type; 
    typedef typename allocator_type::pointer            pointer; 
    typedef typename allocator_type::const_pointer      const_pointer; 
    typedef typename allocator_type::reference          reference; 
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type; 
    typedef typename allocator_type::difference_type    difference_type; 

    typedef value_type*                                 iterator;           // T*
    typedef const value_type*                           const_iterator;     // const T* 
    typedef mystl::reverse_iterator<iterator>           reverse_iterator; 
    typedef mystl::reverse_iterator<const_iterator>     const_reverse_iterator; 

    allocator_type  get_allocator() {return data_allocator(); }

private:  
    iterator _begin; 
    iterator _end; 
    iterator _cap; 

public:
    // ctors 
    vector() noexcept 
    {
        try_init(); 
    }

    explicit vector(size_type n)
    {
        fill_init(n, value_type()); 
    }

    vector(size_type n, const value_type& value)
    {
        fill_init(n, value); 
    }

    // [first, last) 
    template<typename Iter, typename std::enable_if< 
        mystl::is_input_iterator<Iter>::value, int>::type = 0>  
    vector(Iter first, Iter last)
    {
        MYSTL_DEBUG(!(last < first)); 
        range_init(first, last); 
    }

    // copy ctor 
    vector(const vector& rhs)
    {
        range_init(rhs._begin, rhs._end); 
    }

    // move ctor 
    vector(vector&& rhs) noexcept: 
    _begin(rhs._begin), _end(rhs._end), _cap(rhs._cap)
    {
        rhs._begin = nullptr; 
        rhs._end = nullptr; 
        rhs._cap = nullptr; 
    }

    // initialized list 
    vector(std::initializer_list<value_type> initlist)
    {
        range_init(initlist.begin(), initlist.end()); 
    }

    // assign 
    vector& operator= (const vector& rhs); 
    vector& operator= (vector&& rhs) noexcept; 

    vector& operator= (std::initializer_list<value_type> initlist)
    {
        vector temp(initlist.begin(), initlist.end()); 
        this->swap(temp); 
        return *this; 
    }

    // dtor 
    ~vector() 
    {
        destroy_and_recover(_begin, _end, _cap - _begin); 
        _begin = nullptr; 
        _end = nullptr; 
        _cap = nullptr; 
    }

public: 

    // iterator options 
    iterator begin()                noexcept {return _begin; }
    const_iterator begin()    const noexcept {return _begin;} 

    iterator end()                  noexcept {return _end; }
    const_iterator end()      const noexcept {return _end; }

    reverse_iterator rbegin()               noexcept
    {return reverse_iterator(end());} 
    const_reverse_iterator rbegin()   const noexcept 
    {return const_reverse_iterator(end());}

    reverse_iterator rend()                 noexcept
    {return reverse_iterator(begin());}
    const_reverse_iterator rend()     const noexcept
    {return const_reverse_iterator(begin()); }

    const_iterator cbegin()         const noexcept
    {return begin(); }
    const_iterator cend()           const noexcept
    {return end(); }
    const_reverse_iterator  crbegin()    const noexcept 
    {return rbegin(); }
    const_reverse_iterator  crend()      const noexcept 
    {return rend(); }


    // 容量相关
    bool empty() const noexcept
    {
        return _begin == _end; 
    }

    size_type   size()  const noexcept
    {
        return static_cast<size_type>(_end - _begin); 
    }

    size_type max_size() const noexcept 
    {
        return static_cast<size_type>(-1) / sizeof(T); 
    }

    size_type capacity() const noexcept 
    {
        return static_cast<size_type>(_cap - _begin); 
    }


    void reserve(size_type new_cap);
    void shrink_to_fit(); 

    // 元素访问操作
    reference   operator[] (size_type n)
    {
        MYSTL_DEBUG(n < size());
        return *(_begin + n); 
    }

    const reference operator[] (size_type n) const 
    {
        MYSTL_DEBUG(n < size()); 
        return *(_begin + n); 
    }

    // bound check 
    reference at(size_type n )
    {
        THROW_OUT_OF_RANGE_IF( !(n < size()) , "vector<T>at() subscript out of range");
        return (*this)[n]; // calls operator[] 
    }

    const_reference at(size_type n) const 
    {
        THROW_OUT_OF_RANGE_IF( !(n < size()) , "vector<T>at() subscript out of range");
        return (*this)[n]; // calls operator[] 
    }

    reference front() 
    {
        MYSTL_DEBUG(!empty()); 
        return *begin(); 
    }

    const_reference front() const 
    {
        MYSTL_DEBUG(!empty()); 
        return *begin(); 
    }

    reference back() 
    {
        MYSTL_DEBUG(!empty());
        return *(_end - 1); 
    }

    const_reference back() const 
    {
        MYSTL_DEBUG(!empty()); 
        return *(_end - 1); 
    }

    pointer data() noexcept
    {
        return _begin; 
    }
    const pointer data() const noexcept 
    {
        return _begin; 
    }

    // 修改容器操作
    // assign

    void assign(size_type n, const value_type& value)
    {
        fill_assign(n, value); 
    }

    template <typename Iter, typename std::enable_if< 
        mystl::is_input_iterator<Iter>::value, int> :: type = 0>  
    void assign(Iter first, Iter last)
    {
        MYSTL_DEBUG(!(last < first)); 
        copy_assign(first, last, iterator_category(first)); 
    }

    void assign(std::initializer_list<value_type> initlist)
    {
        copy_assign(initlist.begin(), initlist.end(), std::forward_iterator_tag{}); 
    }

    // emplace / emplace_back 
    template<typename... Args>  
    iterator emplace (const_iterator pos, Args&& ...args); 

    template <typename... Args>   
    void emplace_back (Args&& ...args); 

    //push_back & pop_back 

    void push_back(const value_type& value); 
    void push_back(value_type && value)
    {
        emplace_back(std::move(value)); 
    }
    void pop_back(); 

    // insert 
    iterator insert(const_iterator pos, const value_type& value);
    iterator insert(const_iterator pos, value_type&& value)
    {
        return emplace(pos, std::move(value)); 
    }

    // insert n elements in pos 
    iterator insert(const_iterator pos, size_type n, const value_type& value)
    {
        MYSTL_DEBUG(pos >= begin() && pos <= end() ); 
        return fill_insert(const_cast<iterator>(pos), n, value); 
    }

    template<typename Iter, typename std::enable_if< 
        mystl::is_input_iterator<Iter>::value, int>::type = 0>   
    void insert(const_iterator pos, Iter first, Iter last)
    {
        MYSTL_DEBUG(pos>= begin() && pos <= end() && !(last < first)); 
        copy_insert(const_cast<iterator>(pos), first, last); 
    }

    // erase && clear
    iterator erase(const_iterator pos); 
    iterator erase(const_iterator first, const_iterator last); 
    void clear() {erase(begin(), end()); }

    // resize && reverse 
    void resize(size_type new_size) {return resize(new_size, value_type());}
    void resize(size_type new_size, const value_type& value); 

    void reverse() {std::reverse(begin(), end()); }

    // swap 
    void swap(vector& rhs) noexcept; 

private:  
    // auxiliary methods 
    // initialize && destroy 

    void try_init() noexcept; 

    void init_space(size_type size, size_type cap); 

    void fill_init(size_type n, const value_type& value); 

    template <typename Iter>   
    void range_init(Iter first, Iter last); 

    void destroy_and_recover(iterator first, iterator last, size_type n); 

    // calculate grow size 
    size_type get_new_capacity(size_type add_size); 

    // assign 
    void fill_assign(size_type n, const value_type& value); 

    template <typename InputIter>
    void copy_assign(InputIter first, InputIter last, input_iterator_tag); 

    template <typename ForwardIter>   
    void copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag); 

    // reallocate 
    template <typename... Args>
    void reallocate_emplace(iterator pos, Args&& ...args); 

    void reallocate_insert(iterator pos, const value_type& value); 

    // shrink_to_fit
    void reinsert(size_type size); 

    iterator fill_insert(iterator pos, size_type n, const value_type& value);
    template <typename InputIter>  
    void copy_insert(iterator pos, InputIter first, InputIter last); 

}; 

// copy assign 
template <typename T> 
vector<T>& vector<T>::operator= (const vector& rhs)
{
    if(this != &rhs)
    {
        const auto len = rhs.size(); 
        // 1 长度超出了capacity
        if(len > capacity())
        {
            vector temp(rhs.begin(), rhs.end()); 
            swap(temp); 
        }
        else if(size() >= len)  // 2. 在size范围内能够容纳
        {
            auto ite = std::copy(rhs.begin(), rhs.end(), begin()); 
            data_allocator::destroy(ite, end()); 
            _end = _begin + len; 
        }
        else
        {
            std::copy(rhs.begin(), rhs.begin() + size(), _begin); 
            mystl::uninitialized_copy(rhs.begin()+ size(), rhs.end(), _end);
            _cap = _end = _begin + len;  
        }
    }
    return *this; 
}

// move assign 
template <typename T> 
vector<T>& vector<T>::operator=(vector&& rhs) noexcept
{
    destroy_and_recover(_begin, _end, _cap - _begin); 
    _begin = rhs._begin; 
    _end = rhs._end; 
    _cap = rhs._cap; 
    rhs._begin = nullptr; 
    rhs._end = nullptr; 
    rhs._cap = nullptr; 
    return *this; 
}

// reverse: reallocate when n is larger than capacity 
template <typename T> 
void vector<T>:: reserve(size_type n)
{
    if(capacity() < n)
    {
        THROW_LENGTH_ERROR_IF(n > max_size(),
             "n cannot be larger than max_size() is vector<T>"); 
        const auto old_size = size(); 
        auto temp = data_allocator::allocate(n); //     reallocate n 
        mystl::uninitialized_move(_begin, _end, temp); 
        data_allocator::deallocate(_begin, _cap - _begin); 
        _begin = temp; 
        _end = temp + old_size; 
        _cap = _begin + n; 
    }
}

// shrink to fit 
template<typename T> 
void vector<T>:: shrink_to_fit()
{
    if(_end < _cap)
    {
        reinsert(size()); 
    }
}

// emplace 
template <typename T> 
template <typename ...Args> 
typename vector<T>::iterator 
vector<T>::emplace(const_iterator pos, Args&& ...args)
{
    MYSTL_DEBUG(pos>=begin() && pos <= end()); 
    iterator xpos = const_cast<iterator>(pos); 
    const size_type n = xpos - _begin; 
    
    // emplace_back 
    if(_end != _cap && xpos == _end)
    {
        data_allocator::construct(mystl::address_of(*_end), mystl::forward<Args>(args)...); 
        ++_end; 
    }

    else if(_end != _cap) 
    {
        //auto new_end = _end; 
        data_allocator::construct(mystl::address_of(*_end), *(_end - 1)); 
        ++_end; 
        std::copy_backward(xpos, _end - 1, _end); // first, last, result 
        *xpos = value_type(mystl::forward<Args>(args)...); // value_type ctor 
    }
    else 
    {
        reallocate_emplace(xpos, mystl::forward<Args>(args)...); 
    }
    return _begin + n; 
}

// empalce_back 
template <typename T>  
template <typename ...Args>  
void vector<T>:: emplace_back(Args&& ...args)
{
    if(_end < _cap)
    {
        data_allocator::construct(mystl::address_of(*_end), mystl::forward<Args>(args)...); 
        ++_end;
    }
    else
    {
        reallocate_emplace(_end, mystl::forward<Args>(args)...); 
    }
}

// push_back 
template <typename T> 
void vector<T>::push_back(const value_type& value)
{
    if(_end != _cap)
    {
        data_allocator::construct(mystl::address_of(*_end), value); 
        ++_end; 
    }
    else 
    {
        // reallocate 
        reallocate_insert(_end, value); 
    }
}

// pop_back 
template <typename T> 
void vector<T>::pop_back()
{
    MYSTL_DEBUG(!empty()); 
    data_allocator::destroy(_end - 1);
    --_end; 
}

// insert at pos 
template <typename T> 
typename vector<T>::iterator 
vector<T>::insert(const_iterator pos, const value_type& value)
{
    MYSTL_DEBUG(pos >= begin() && pos <= end()); 
    iterator xpos = const_cast<iterator>(pos); 
    const size_type n = pos - _begin; 
    if(_end != _cap && xpos == _end)
    {
        data_allocator::construct(mystl::address_of(*_end), value); 
        ++_end; 
    }
    else if (_end != _cap) 
    {
        auto new_end = _end; 
        data_allocator::construct(mystl::address_of(*_end), *(_end - 1)); 
        ++new_end; 
        auto value_copy = value; 
        std::copy_backward(xpos, _end - 1, _end); 
        *xpos = mystl::move(value_copy);
        _end = new_end; 
    }
    else 
    {
        reallocate_copy(xpos, value); 
    }
    return _begin + n; 
}

// erase pos 
template <typename T> 
typename vector<T> :: iterator 
vector<T>::erase(const_iterator pos)
{
    MYSTL_DEBUG(pos >= begin() && pos < end()); 
    iterator xpos = _begin + (pos - begin()); 
    // move 要求迭代器不能是const_iterator 
    std::move(xpos + 1, _end, xpos); 
    data_allocator::destroy(_end -1); 
    --_end; 
    return xpos; 
}

// 区间删除算法
template <typename T> 
typename vector<T>::iterator 
vector<T> ::erase(const_iterator first, const_iterator last)
{
    MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first)); 
    const auto n = first - begin(); 

    iterator r = _begin + n; 
    data_allocator::destroy(std::move(r + (last - first), _end, r), _end); 
    _end = _end - (last - first); 
    return _begin + n; 
}

// resize 
template <typename T> 
void vector<T>::resize(size_type new_size, const value_type& value)
{
    if(new_size < size())
    {
        // 区间删除
        erase(begin() + new_size, end()); 
    }
    else 
    {
        // 插入补齐
        insert(end(), new_size - size(), value); 
    }
}

// swap with another vector<T> 
template <typename T> 
void vector<T>::swap (vector<T>& rhs) noexcept 
{
    if(this != &rhs)
    {
        mystl::swap(_begin, rhs._begin); 
        mystl::swap(_end, rhs._end);
        mystl::swap(_cap, rhs._cap); 
    }
}

// auxiliary methods 

// try_init 分配失败忽略，不抛出异常 默认 16个元素
template<typename T>  
void vector<T>::try_init() noexcept
{
    try
    {
        _begin = data_allocator::allocate(16); 
        _end  = _begin; 
        _cap = _begin + 16; 
    }
    catch(...)
    {
        _begin = nullptr; 
        _end = nullptr; 
        _cap = nullptr; 
    }
}

// init_space 指定空间大小
template<typename T> 
void vector<T>::init_space(size_type size, size_type capacity)
{
    try
    {
        _begin = data_allocator::allocate(capacity); 
        _end = _begin + size; 
        _cap = _begin + capacity;
    }
    catch(...)
    {
        _begin = nullptr; 
        _end = nullptr; 
        _cap = nullptr; 
        throw; 
    }
}

// fill_init 
template<typename T> 
void vector<T>:: fill_init(size_type n, const value_type& value)
{
    const size_type init_size = std::max(static_cast<size_type>(16), n); 
    init_space(n, init_size); 
    mystl::uninitialized_fill_n(_begin, n, value); 
}

// range_init 
template <typename T> 
template <typename Iter> 
void vector<T>:: range_init(Iter first, Iter last)
{
    // 这样填充后的capacity 最小也是16 
    const size_type init_size = std::max(static_cast<size_type>(16), 
                                         static_cast<size_type>(last - first));    
    init_space(static_cast<size_type>(last -first), init_size); 
    mystl::uninitialized_copy_n(first, last, _begin); 
}

// destroy and recover 销毁对象并回收空间
template <typename T> 
void vector<T>:: destroy_and_recover (iterator first, iterator last, size_type n) 
{
    data_allocator::destroy(first, last); 
    data_allocator::deallocate(first, n); 
}

// get new capacity 扩容策略
template <typename T> 
typename vector<T>:: size_type
vector<T>:: get_new_capacity (size_type add_size)
{
    // 应该转用加倍策略 
    const auto old_size = capacity(); 
    THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size, "vector<T>'s size too big"); 
    if(old_size > max_size() - old_size/2)
    {
        return old_size + add_size > max_size() - 16 ? 
                old_size + add_size : old_size + add_size + 16; 
    }

    const size_type new_size = 0 == old_size  ?  
        std::max(add_size, static_cast<size_type>(16)) : 
        std::max(old_size + old_size/2, old_size + add_size); 

    return new_size;
}

// fill_assign 
template <typename T> 
void vector<T>::  fill_assign(size_type n, const value_type& value)
{
    if(n > capacity())
    {
        vector temp(n, value); 
        swap(temp); 
    }
    else if(n > size())
    {
        std::fill(begin(), end(), value); 
        _end = mystl::uninitialized_fill_n(_end, n - size(), value); 
    }
    else 
    {
        erase(std::fill_n(_begin, n, value), _end); 
    }
}

// copy_assign 
template<typename T>   
template <typename InputIter> 
void vector<T>:: copy_assign(InputIter first, InputIter last, input_iterator_tag)
{
    auto curr = _begin; 
    for(; first != last && curr != _end; ++first, ++curr )
    {
        *curr = *first; 
    }
    if(first == last)
    {
        erase(curr, _end); 
    }
    else 
    {
        insert(_end, first, last); 
    }
}

// copy_assign: [first, last) 
template <typename T>
template <typename ForwardIter> 
void vector<T>::copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag)
{
    const size_type len = mystl::distance(first, last); 
    if(len > capacity())
    {
        vector temp(first, last); 
        swap(temp); 
    }

    else if(size() >= len)
    {
        auto new_end = std::copy(first, last, _begin); 
        data_allocator::destroy(new_end, _end); 
        _end = new_end; 
    }

    else
    {
        auto mid = first; 
        mystl::advance(mid, size()); 
        std::copy(first, mid, _begin); 
        auto new_end = mystl::uninitialized_copy_n(mid, last, _end); 
        _end = new_end; 
    }
}

// reallocate_emplace 重新分配空间并在pos处就地构造元素
template <typename T>
template<typename... Args> 
void vector<T>::  
reallocate_emplace(iterator pos, Args&& ...args)
{
    const auto new_size = get_new_capacity(1); 
    auto new_begin = data_allocator::allocate(new_size); 
    auto new_end = new_begin; 

    try
    {
        new_end = mystl::uninitialized_move(_begin, pos, new_begin);
        data_allocator::construct(mystl::address_of(*new_end), mystl::forward<Args>(args)...); 
        ++new_end; 
        new_end = mystl::uninitialized_move(pos, _end, new_end); 
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, new_size); 
        throw; 
    }
    
    destroy_and_recover(_begin, _end, _cap - _begin); 
    _begin = new_begin; 
    _end = new_end; 
    _cap = new_begin + new_size; 
}

// reallocate_insert 重新分配空间并在 pos 处插入元素
template <typename T> 
void vector<T>::reallocate_insert(iterator pos, const value_type& value)
{
    const auto new_size = get_new_capacity(1); 
    auto new_begin = data_allocator::allocate(new_size); 
    auto new_end = new_begin; 
    const value_type& value_copy = value; 

    try
    {
        new_end = mystl::uninitialized_move(_begin, pos, new_begin);
        data_allocator::construct(mystl::address_of(*new_end), value_copy); 
        ++new_end; 
        new_end = mystl::uninitialized_move(pos, _end, new_end); 
        
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, new_size); 
        throw; 
    }

    destroy_and_recover(_begin, _end, _cap - _begin); 
    _begin = new_begin; 
    _end = new_end; 
    _cap = new_begin + new_size; 
}

// fill_insert 
template<typename T> 
typename vector<T>::iterator 
vector<T>:: fill_insert(iterator pos, size_type n, const value_type& value)
{
    if(n == 0)
    {
        return pos; 
    }
    const size_type xpos = pos - _begin; 
    const value_type value_copy = value; 

    if(static_cast<size_type>(_cap - _end) >= n)
    {
        // 不需要扩容
        const size_type after_elems = _end - pos; 
        auto old_end = _end; 
        if(after_elems > n)  
        {
            mystl::uninitialized_copy(_end - n, _end, _end); 
            _end += n; 
            std::move_backward(pos, old_end - n, old_end); 
            mystl::uninitialized_fill_n(pos, n, value_copy); 
        }
        else  
        {
            _end = mystl::uninitialized_fill_n(_end, n - after_elems, value_copy);
            _end = mystl::uninitialized_move(pos, old_end, _end); 
            mystl::uninitialized_fill_n(pos, after_elems, value_copy); 
        }
    }
    else    
    {
        // 需要扩容
        const auto new_size = get_new_capacity(n); 
        auto new_begin = data_allocator::allocate(new_size); 
        auto new_end = new_begin; 

        try
        {
            new_end = mystl::uninitialized_move(_begin, pos, new_begin);
            new_end = mystl::uninitialized_fill_n(new_end, n, value); 
            new_end = mystl::uninitialized_move(pos, _end, new_end); 
        }
        catch(...)
        {
            destroy_and_recover(new_begin, new_end, new_size);
            throw; 
        }
        
        data_allocator::deallocate(_begin, _cap - _begin); 
        _begin = new_begin; 
        _end = new_end; 
        _cap = _begin + new_size; 


    }
    return _begin + xpos; 
}

//copy insert 
template <typename T> 
template <typename InputIter>  
void vector<T>::copy_insert(iterator pos, InputIter first, InputIter last)
{
    if(first == last) return; 

    const auto n = mystl::distance(first, last); 
    if(_cap - _end >= n)
    {
        const auto after_elems = _end - pos; 
        auto old_end = _end; 
        if(after_elems > n) 
        {
            _end = mystl::uninitialized_copy(_end - n, _end, _end);
            std::move_backward(pos, old_end -n, old_end); 
            mystl::uninitialized_copy(first, last, pos); 
        }
        else  
        {
            auto mid = first; 
            mystl::advance(mid, after_elems); 
            _end = mystl::uninitialized_copy(mid, last, _end); 
            _end = mystl::uninitialized_move(pos, old_end, _end); 
            mystl::uninitialized_copy(first, mid, pos); 
        }
    }
    else 
    {
        // 备用空间不足
        const auto new_size = get_new_capacity(n); 
        auto new_begin = data_allocator::allocate(new_size); 
        auto new_end = new_begin; 

        try
        {
            new_end = mystl::uninitialized_move(_begin, pos, new_begin); 
            new_end = mystl::uninitialized_copy(first, last, new_end); 
            new_end = mystl::uninitialized_move(pos, _end, new_end); 
        }
        catch(...)
        {
            destroy_and_recover(new_begin, new_end, new_size);
            throw; 
        }
        
        data_allocator::deallocate(_begin, _cap - _begin); 
        _begin = new_begin; 
        _end = new_end; 
        _cap = _begin + new_size; 
    }

}

// reinsert 
template <typename T>  
void vector<T>::reinsert(size_type size)
{

    auto new_begin = data_allocator::allocate(size); 
    try
    {
        mystl::uninitialized_move(_begin, _end, new_begin); 
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, size); 
        throw; 
    }
    
    data_allocator::deallocate(_begin, _cap - _begin); 
    _begin = new_begin; 
    _end = _begin + size; 
    _cap = _begin + size; 
}

// 重载比较运算符
template<typename T> 
bool operator== (const vector<T>& lhs, const vector<T>& rhs)
{
    return lhs.size() == rhs.size() && 
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>  
bool operator< (const vector<T>& lhs, const vector<T>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), 
                        rhs.begin(), rhs.end()); 
}

template<typename T> 
bool operator!= (const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs); 
}

template<typename T>  
bool operator> (const vector<T>& lhs, const vector<T>& rhs)
{
    return rhs < lhs; 
}

template<typename T>  
bool operator<= (const vector<T>& lhs, const vector<T>& rhs)
{
   return !(rhs < lhs);
}

template<typename T>  
bool operator>= (const vector<T>& lhs, const vector<T>& rhs)
{
   return !(lhs < rhs); 
}

// mystl::swap overload 
template <typename T> 
void swap(vector<T>& lhs, vector<T>& rhs)
{
    lhs.swap(rhs); 
}
} // end of namespace mystl 
#endif // !VECTOR_H_ 