// --deque.h 模版类 deque: double end queue 
#ifndef _DEQUE_H_
#define _DEQUE_H_ 

// notes: 
// 异常保证
// mystl::deque<T> 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证
// * emplace_front 
// * emplace_back 
// * emplace 
// * push_front 
// * push_back 
// * insert


#include <initializer_list>
#include <algorithm>
#include "iterator.h"
#include "memory.h"
#include "util.h" 
#include "exceptdef.h" 
#include <memory> 

    // buffer0: [first, last) 
    //             curr                  
    //             |
    // buffer1: [first, last)       <- node 
    // buffer2: [first, last) 

namespace mystl 
{
    // deque 控制中心初始化大小
#ifndef DEQUE_MAP_INIT_SIZE 
#define DEQUE_MAP_INIT_SIZE 8 
#endif 

template <typename T> 
struct deque_buf_size 
{
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16; 
}; 

// deque iterator 
template <typename T, typename Ref, typename Ptr> 
struct deque_iterator: public iterator<random_access_iterator_tag, T> 
{
    typedef     deque_iterator<T, T&, T*>               iterator; 
    typedef     deque_iterator<T, const T&, const T*>   const_iterator; 
    typedef     deque_iterator                          self; 

    typedef T               value_type; 
    typedef Ptr             pointer; 
    typedef Ref             reference; 
    typedef size_t          size_type; 
    typedef ptrdiff_t       difference_type; 
    typedef T*              value_pointer; 
    typedef T**             map_pointer; 

    static const size_type buffer_size = deque_buf_size<T>:: value; 


    // 数据成员：四个指针
    value_pointer   cur;            // 所在缓冲区当前元素
    value_pointer   first;          // 所在缓冲区起始位置
    value_pointer   last;           // 所在缓冲区结束位置
    map_pointer     node;           // 缓冲区所在节点

    // ctor 
    deque_iterator() noexcept 
    : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {} 

    deque_iterator(value_pointer v, map_pointer n) 
    : cur(v), first(*n), last (*n + buffer_size), node(n) {} 

    deque_iterator(const iterator& rhs)
    : cur(rhs.cur), first(rhs.first), last(rhs.last),node(rhs.node){} 

    deque_iterator(iterator&& rhs) noexcept 
    : cur(rhs.cur), first(rhs.first), last(rhs.last),node(rhs.node) 
    {
        rhs.cur = rhs.first = rhs.last = nullptr; 
        rhs.node = nullptr; 
    }

    deque_iterator(const const_iterator& rhs)
    : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {} 

    // operator overloads 
    self& operator=(const iterator& rhs) 
    {
        if(this != &rhs) 
        {
            cur = rhs.cur; 
            first = rhs.first; 
            last = rhs.last; 
            node = rhs.node; 
        }
        return *this; 
    }

    void set_node(map_pointer new_node)
    {
        node = new_node; 
        first = *new_node;
        last = first + buffer_size; 
    }


    reference operator*() const{return *cur; }
    pointer   operator->() const {return cur; }

    // 两个迭代器之间hops 
    difference_type operator - (const self& x) const 
    {
        return static_cast<difference_type>(buffer_size) *(node - x.node) 
            +(cur - first) - (x.cur - x.first); 
    }

    // 
    self& operator++() 
    {
        ++cur; 
        if(cur == last) 
        {
            set_node(node + 1); 
            cur = first; 
        }
        return *this; 
    }

    self operator++(int)
    {
        self temp = *this; 
        ++*this; 
        return temp; 
    }

    self& operator--()
    {
        if(cur == first) 
        {
            set_node(node -  1); 
            cur = last; 
        }
        --cur; 
        return *this; 
    }

    self operator--(int)
    {
        self temp = *this; 
        --*this; 
        return temp; 
    }

    self& operator += (difference_type n)
    {
        const auto offset = n + (cur -first); 
        if(offset >= 0 && offset < static_cast<difference_type>(buffer_size))
        {
            cur +=n; 
        }
        else  
        {
            const auto node_offset = offset >0 ? 
            offset/static_cast<difference_type>(buffer_size):   
            - static_cast<difference_type> ((-offset - 1) / buffer_size) - 1; 
            set_node(node + node_offset); 
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size)); 
        }
        return *this; 
    }

    self operator+(difference_type n) const 
    {
        self temp = *this; 
        return temp += n; 
    }

    self& operator -=(difference_type n) 
    {
        return *this += -n; 
    }

    self operator-(difference_type n) 
    {
        self temp = *this; 
        return temp -=n; 
    }

    // random 迭代器
    reference operator[](difference_type n) const {return *(*this + n);} 

    // relational operator 
    bool operator == (const self& rhs) const {return cur == rhs.cur; }
    bool operator < (const self& rhs) const 
    { return node == rhs.node? (cur < rhs.cur): (node < rhs.node); }
    bool operator != (const self& rhs) const {return !(*this == rhs); }
    bool operator > (const self& rhs) const 
    { return rhs < *this; } 
    bool operator <= (const self& rhs) const 
    { return !(*this > rhs); }
    bool operator >= (const self& rhs) const 
    { return !(*this < rhs); }
}; 


// deque 
template <typename T>   
class deque 
{
public: 
    // deque traits 
    typedef mystl::allocator<T>             allocator_type; 
    typedef mystl::allocator<T>             data_allocator; 
    typedef mystl::allocator<T*>            map_allocator; 

    typedef typename allocator_type::value_type         value_type; 
    typedef typename allocator_type::pointer            pointer; 
    typedef typename allocator_type::const_pointer      const_pointer; 
    typedef typename allocator_type::reference          reference; 
    typedef typename allocator_type::const_reference    const_reference;  
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type; 
    typedef pointer*                                    map_pointer; 
    typedef const pointer*                              const_map_pointer; 

    typedef deque_iterator<T, T&, T*>                   iterator; 
    typedef deque_iterator<T, const T&, const T*>       const_iterator; 
    typedef mystl::reverse_iterator<iterator>           reverse_iterator; 
    typedef mystl::reverse_iterator<const_iterator>     const_reverse_iterator; 

    allocator_type get_allocator() {return allocator_type(); }

    static const size_type buffer_size = deque_buf_size<T>::value; 

private:   
    // 
    iterator _begin;            // 第一个节点   
    iterator _end;              // 末尾节点（哨兵） 
    map_pointer _map;           // T* 数组map  
    size_type   _map_size;      // map 中元素个数 

public:  
    // ctor 
    deque() 
    { fill_init(0, value_type()); }

    explicit deque(size_type n)
    { fill_init(n, value_type()); }

    deque(size_type n, const value_type& value)
    { fill_init(n, value);} 

    template <typename InputIter, typename std::enable_if < 
    mystl::is_input_iterator<InputIter>::value, int>::type = 0 >  
    deque(InputIter first, InputIter last) 
    { copy_init(first, last, iterator_category(first)); } 

    deque(std::initializer_list<T> initlist) 
    { 
        copy_init(initlist.begin(), initlist.end(), mystl::forward_iterator_tag());
    } 

    deque(const deque& rhs) 
    {
        copy_init(rhs.begin(), rhs.end(), mystl::forward_iterator_tag()); 
    }

    deque(deque&&rhs ) noexcept 
    : _begin(rhs._begin), _end(rhs._end), _map(rhs._map), _map_size(rhs._map_size) 
    {
        rhs._map = nullptr; 
        rhs._map_size = 0; 
    }

    // copy assign 
    deque& operator= (const deque& rhs); 
    deque& operator= (deque && rhs); 

    deque& operator=(std::initializer_list<value_type> initlist)
    {
        deque temp(initlist);
        swap(temp); 
        return *this; 
    }

    ~deque() 
    {
        if(_map != nullptr) 
        {
            clear(); 
            //clear 之后，只剩下一个buffer没有被释放
            data_allocator::deallocate(*_begin.node, buffer_size);
            *_begin.node = nullptr; 
            map_allocator::deallocate(_map, _map_size); 
            _map = nullptr; 
        }
    }

public: 
    // 迭代器相关操作

    iterator    begin()     noexcept 
    {return _begin; }
    const_iterator  begin() const noexcept 
    {return _begin; } 
    iterator    end()       noexcept 
    {return _end; } 
    const_iterator  end()   const noexcept 
    {return _end; } 

    reverse_iterator    rbegin()    noexcept 
    {return reverse_iterator(end()); } 
    const_reverse_iterator  rbegin()    const noexcept 
    {return reverse_iterator(end); }     
    reverse_iterator    rend()      noexcept 
    {return reverse_iterator(begin()); } 
    const_reverse_iterator rend() const noexcept 
    {return reverse_iterator(begin()); } 

    const_iterator      cbegin()    const noexcept 
    {return begin(); } 
    const_iterator      cend()      const noexcept 
    {return end(); } 
    const_reverse_iterator crbegin() const noexcept 
    {return rbegin(); } 
    const_reverse_iterator crend()  const noexcept 
    {return rend(); } 

    // 涉及容量的操作 
    bool empty() const noexcept {return begin() == end(); } 
    size_type size()    const noexcept {return _end - _begin; } 
    size_type max_size() const noexcept {return static_cast<size_type>(-1); }
    void resize(size_type new_size) { resize(new_size, value_type()); } 
    void resize(size_type new_size, const value_type& value); 
    void shrink_to_fit() noexcept; 

    // visitor 元素访问
    reference   operator[](size_type n) 
    {
        MYSTL_DEBUG(n < size()); 
        return _begin[n]; 
    }

    const_reference operator[] (size_type n) const 
    {
        MYSTL_DEBUG(n < size()); 
        return _begin[n]; 
    }

    reference at(size_type n)
    {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscirpt out of range"); 
        return _begin[n]; 
    } 

    const_reference at(size_type n ) const 
    {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range"); 
        return _begin[n]; 
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
        return *(end() - 1); 
    }
    
    const_reference back() const 
    {
        MYSTL_DEBUG(!empty());
        return *(end() - 1); 
    }

    // 修改容器相关操作 

    // assign 
    void assign (size_type n, const value_type& value)
    { fill_assign(n, value); }

    template<typename InputIter, typename std::enable_if < 
        mystl::is_input_iterator<InputIter>::value, int>::type = 0>  
    void assign(InputIter first, InputIter last) 
    { copy_assign(first, last, iterator_category(first)); } 

    void assign(std::initializer_list<value_type> initlist) 
    { copy_assign(initlist.begin(), initlist.end(), mystl::forward_iterator_tag()); } 

    // emplace_front / emplace_back / emplace 
    template <typename ... Args>   
    void emplace_front(Args&& ... args); 

    template <typename ... Args>  
    void emplace_back(Args&& ...args); 

    template <typename ... Args>  
    iterator emplace(iterator pos, Args&& ... args); 

    // push_front / push_back 
    void push_front(const value_type& value); 
    void push_back(const value_type& value); 

    void push_front(value_type&& value) {emplace_front(mystl::move(value)); } 
    void push_back(value_type&& value) {emplace_back(mystl::move(value)); } 

    // pop_back / pop_front 
    void pop_back(); 
    void pop_front(); 

    //insert  
    iterator insert(iterator pos, const value_type& value); 
    iterator insert(iterator pos, value_type&& value); 
    void insert(iterator pos, size_type n, const value_type& value); 
    template<typename InputIter, typename std::enable_if<mystl::is_input_iterator < 
        InputIter>::value, int>::type = 0>   
    void insert(iterator pos, InputIter first, InputIter last) 
    { insert_dispatch(pos, first, last, iterator_category(first)); } 

    // erase / clear 
    iterator erase(iterator pos); 
    iterator erase(iterator first, iterator last); 
    void clear(); 

    // swap 
    void swap(deque& rhs) noexcept; 

private: 
    // auxiliary methods 

    // create / destroy node 
    map_pointer create_map(size_type size); 
    void create_buffer(map_pointer nstart, map_pointer nfinish); 
    void destroy_buffer(map_pointer nstart, map_pointer nfinish); 

    // initialize  
    void map_init(size_type nelems); 
    void fill_init(size_type n, const value_type& value); 
    template<typename InputIter>   
    void copy_init(InputIter first, InputIter last, input_iterator_tag); 
    template <typename ForwardIter>   
    void copy_init(ForwardIter first, ForwardIter last, forward_iterator_tag); 

    // assign  
    void fill_assign(size_type n, const value_type& value); 
    template <typename InputIter>   
    void copy_assign(InputIter first, InputIter last, input_iterator_tag); 
    template <typename ForwardIter>  
    void copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag); 
    
    // insert 
    template <typename ... Args>
    iterator    insert_aux(iterator position, Args&& ...args);
    void        fill_insert(iterator position, size_type n, const value_type& x);
    template <typename FIter>
    void        copy_insert(iterator, FIter, FIter, size_type);
    template <typename IIter>
    void        insert_dispatch(iterator, IIter, IIter, input_iterator_tag);
    template <typename FIter>
    void        insert_dispatch(iterator, FIter, FIter, forward_iterator_tag);


    // reallocate  
    void require_capacity(size_type n, bool isFront); 
    void reallocate_map_at_front(size_type need); 
    void reallocate_map_at_back(size_type need); 
}; 

// copy assign 
template <typename T> 
deque<T>& deque<T>:: operator=(const deque& rhs) 
{
    if(this != &rhs)
    {
        const auto len = size(); 
        if(len >= rhs.size())
        {
            erase(std::copy(rhs._begin, rhs._end, _begin). _end); 
        }
        else   
        {
            iterator mid = rhs.begin() + static_cast<difference_type> (len); 
            std::copy(rhs._begin, mid, _begin); 
            insert(_end, mid, rhs._end); 

        }
    }
    return *this; 
}

// move assign 
template <typename T>  
deque<T>& deque<T>::operator=(deque&& rhs )
{
    clear(); 
    _begin = mystl::move(rhs._begin); 
    _end  = mystl::move(rhs._end); 
    _map = rhs._map; 
    _map_size = rhs._map_size; 
    rhs._map = nullptr; 
    rhs._map_size = 0; 
    return *this;     
}

// resize container 
template <typename T> 
void deque<T>::shrink_to_fit() noexcept 
{
    // at least leave head buffer 
    for(auto cur = _map; cur < _begin.node; ++cur) 
    {
        data_allocator::deallocate(*cur, buffer_size); 
 
    }
    // fixed bug, cur should start from _end.node, not _end.node + 1
    for(auto cur = _end.node; cur < _map + _map_size; ++cur) 
    {
        data_allocator::deallocate(*cur, buffer_size); 
        *cur = nullptr; 
    }
}

// emplace_front 
template <typename T> 
template <typename ...Args>   
void deque<T>::emplace_front(Args&&...args) 
{
    if(_begin.cur != _begin.first)
    {
        data_allocator::construct(_begin.cur -1, mystl::forward<Args>(args)...); 
        --_begin.cur; 
    }
    else  
    {
        // 重新分配空间
        require_capacity(1, true); 
        try
        {
            --_begin; 
            data_allocator::construct(_begin.cur, mystl::forward<Args>(args)...); 
        }
        catch(...)
        {
            ++_begin; 
            throw; 
        }
    }
}

// emplace at back 
template <typename T>  
template <typename ...Args> 
void deque<T>::emplace_back(Args&&...args) 
{
    if(_end.cur != _end.last - 1) 
    {
        data_allocator::construct(_end.cur, mystl::forward<Args>(args)...); 
        ++_end.cur; 
    }
    else  
    {
        require_capacity(1, false); 
        data_allocator::construct(_end.cur, mystl::forward<Args>(args)...); 
        ++_end; // 跨越了buffer 不连续 
    }
}

// pos 处就地构造元素 
template<typename T> 
template<typename ... Args>  
typename deque<T>::iterator deque<T>::emplace(iterator pos, Args&&... args) 
{
    if(pos == _begin.cur)
    {
        emplace_front(mystl::forward<Args>(args)...); 
        return _begin; 
    }
    else if(pos.cur == _end.cur)
    {
        emplace_back(mystl::forward<Args>(args)...); 
        return _end - 1; 
    }
    return insert_aux(pos, mystl::forward<Args>(args)...); 
}

// push_front 
template <typename T> 
void deque<T>::push_front(const value_type& value) 
{
    if(_begin.cur != _begin.first)
    {
        data_allocator::construct(_begin.cur-1, value); 
        --_begin.cur; 
    }
    else 
    {
        require_capacity(1, true); 
        try
        {
            --_begin; 
            data_allocator::construct(_begin.cur, value); 
        }
        catch(...)
        {
            ++_begin; 
            throw; 
        }
    }
}

// push_back
template <typename T> 
void deque<T>::push_back(const value_type& value)
{
    if(_end.cur != _end.last - 1)
    {
        data_allocator::construct(_end.cur, value); 
        ++_end.cur; 
    }
    else 
    {
        require_capacity(1, false); 
        data_allocator::construct(_end.cur, value); 
        ++_end; 
    }
}

// pop front 
template <typename T>  
void deque<T>::pop_front() 
{
    MYSTL_DEBUG(!empty()); 
    if(_begin.cur != _begin.last - 1) 
    {
        data_allocator::destroy(_begin.cur); 
        ++_begin.cur; 
    }
    else   
    {
        data_allocator::destroy(_begin.cur); 
        ++_begin; 
        destroy_buffer(_begin.node - 1, _begin.node - 1); 
    }
}

// pop back  
template <typename T>  
void deque<T>::pop_back () 
{
    MYSTL_DEBUG(!empty()); 
    if(_end.cur != _end.first)
    {
        --_end.cur; 
        data_allocator::destroy(_end.cur); 
    }
    else  
    {
        --_end; 
        data_allocator::destroy(_end.cur); 
        destroy_buffer(_end.node+1, _end.node + 1); 
    }
}

// insert at pos 
template <typename T> 
typename deque<T>::iterator  
deque<T>:: insert(iterator pos, const value_type& value) 
{
    if(pos.cur == _begin.cur)
    {
        push_front(value); 
        return _begin; 
    }
    else if(pos.cur == _end.cur) 
    {
        push_back(value); 
        auto temp = _end; 
        --temp; 
        return temp; 
    }
    else  
    {
        return insert_aux(pos, value); 
    }
}

template <typename T>  
typename deque<T>::iterator  
deque<T>::insert(iterator pos, value_type&& value) 
{
    if(pos.cur == _begin.cur)
    {
        emplace_front(mystl::move(value)); 
        return _begin; 
    }
    else  if(pos.cur == _end.cur )
    {
        emplace_back(mystl::move(value)); 
        auto temp = _end; 
        --temp;
        return temp;  
    }
    else  
    {
        return insert_aux(pos, mystl::move(value));  
    }
}

// insert n elems at pos  
template <typename T>  
void deque<T>::insert(iterator pos, size_type n, const value_type& value) 
{
    if(pos.cur == _begin.cur)
    {
        require_capacity(n, true); 
        auto new_begin = _begin - n; 
        mystl::uninitialized_fill_n(new_begin, n, value); 
        _begin = new_begin; 
    }
    else if(pos.cur == _end.cur)
    {
        require_capacity(n, false); 
        auto new_end = _end + n; 
        mystl::uninitialized_fill_n(_end, n, value); 
        _end = new_end; 
    }
    else  
    {
        fill_insert(pos, n, value);  
    }
}

// erase elem at pos  
template <typename T>  
typename deque<T>::iterator  
deque<T>::erase(iterator pos) 
{
    auto next = pos; 
    ++next; 
    const size_type elems_before = pos - _begin; 
    if(elems_before < size() / 2) 
    {
        std::copy_backward(_begin, pos, next); 
        pop_front(); 
    }
    else  
    {   
        std::copy(next, _end, pos); 
        pop_back(); 
    }
}

// erase [first, last)  
template <typename T>  
typename deque<T>::iterator 
deque<T>::erase(iterator first, iterator last) 
{
    if(first == _begin && last == _end)
    {
        clear(); 
        return _end; 
    }
    else 
    {
        const size_type len = last - first; 
        const size_type elems_before = first - _begin; 
        if(elems_before < (size() - len)/ 2) 
        {
            std::copy_backward(_begin, first, last); 
            auto new_begin = _begin + len; 
            data_allocator::destroy(_begin.cur, new_begin.cur); 
            _begin = new_begin;  
        }
        else  
        { 
            std::copy(last, _end, first); 
            auto new_end = _end - len; 
            data_allocator::destroy(new_end.cur, _end.cur); 
            _end = new_end; 
        }
        return _begin + elems_before; 
    }
}

// clear deque 
template <typename T>  
void deque<T>:: clear()  
{
    // clear keeps only head buffer objects(elements) alive  
    for(map_pointer cur = _begin.node + 1; cur < _end.node; ++cur)
    {
        data_allocator::destroy(*cur, *cur + buffer_size); 
    }

    // more than one buffer 
    if(_begin.node != _end.node) 
    {
        mystl::destroy(_begin.cur, _begin.last); 
        mystl::destroy(_end.first, _end.cur); 
    }
    else  
    {
        mystl::destroy(_begin.cur, _end.cur); 
    }
    shrink_to_fit(); 
    _end = _begin; 
}

// swap two deques  
template <typename T> 
void deque<T>:: swap(deque& rhs) noexcept  
{
    if(this != &rhs) 
    {
        std::swap(_begin, rhs._begin); 
        std::swap(_end, rhs._end); 
        std::swap(_map, rhs._map); 
        std::swap(_map_size. rhs._map_size); 
    }
}

// auxiliary methods 

//  create_map; 
template <typename T> 
typename deque<T>::map_pointer  
deque<T>::create_map(size_type size) 
{
    map_pointer mp = nullptr; 
    mp = map_allocator::allocate(size);  
    for(auto i = 0; i < size; ++i)   
    {
        mp[i] = nullptr; 
    }
    return mp; 
}

// create buffer  
template <typename T>  
void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinish) 
{
    map_pointer cur; 
    try
    {
        for(cur = nstart; cur <= nfinish; ++cur) 
        {
            *cur = data_allocator::allocate(buffer_size); 
        }
    }
    catch(...)
    {
        while(cur != nstart)
        {
            --cur; 
            data_allocator::deallocate(*cur, buffer_size); 
            *cur = nullptr; 
        }
        throw; 
    }
}

//destroy_buffer 
template <typename T> 
void deque<T>::destroy_buffer(map_pointer nstart, map_pointer nfinish)
{
    for(map_pointer n= nstart; n <= nfinish; ++n)
    {
        data_allocator::deallocate(*n, buffer_size); 
        *n = nullptr; 
    }
}

// map init  
template <typename T>  
void deque<T>:: map_init(size_type nElems) 
{
    const size_type nNodes = nElems / buffer_size + 1; 
    _map_size = std::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nNodes + 2); 
    try
    {
        _map  = create_map(_map_size); 
    }
    catch(...)
    {
        _map = nullptr; 
        _map_size = 0; 
        throw; 
    }
      
    // 让nstart 和 nend 指向map 中间，便于向两侧扩充 
    map_pointer nstart = _map + (_map_size - nNodes) / 2; 
    map_pointer nfinish = nstart +nNodes - 1; 

    try
    {
        create_buffer(nstart, nfinish); 
    }
    catch(...)
    {
        map_allocator::deallocate(_map, _map_size); 
        _map = nullptr; 
        _map_size = 0; 
        throw; 
    }
    
    _begin.set_node(nstart); 
    _end.set_node(nfinish); 
    _begin.cur = _begin.first; 
    _end.cur = _end.first + (nElems % buffer_size); 
}

// fill_init 
template <typename T> 
void deque<T>:: fill_init(size_type n, const value_type& value) 
{
    map_init(n); 
    if(n != 0) 
    {
        for(auto cur = _begin.node; cur < _end.node; ++cur)
        {
            std::uninitialized_fill(*cur, *cur + buffer_size, value); 
        }
        std::uninitialized_fill(_end.first, _end.cur, value); 
    }
}

// copy_init 
template <typename T> 
template <typename InputIter>   
void deque<T>:: copy_init(InputIter first, InputIter last, input_iterator_tag) 
{
    const size_type n = mystl::distance(first, last);  
    map_init(n); 
    for(; first != last; ++first) 
    {
        emplace_back(*first); 
    }
}

// copy_init: forward_iterator  
template <typename T>  
template <typename ForwardIter>  
void deque<T>:: copy_init(ForwardIter first, ForwardIter last, forward_iterator_tag)
{
    const size_type n = mystl::distance(first, last); 
    map_init(n); 
    for(auto cur = _begin.node; cur < _end.node; ++cur) 
    {
        auto next = first; 
        mystl::advance(next, buffer_size); 
        mystl::uninitialized_copy(first, next, *cur); 
    }
    mystl::uninitialized_copy(first, last, _end.first); 
}

// fill_assign 
template<typename T>  
void deque<T>::fill_assign(size_type n, const value_type& value) 
{
    if( size() < n) 
    {
        std::fill(begin(), end(), value); 
        insert(end(), n - size(), value); 
    }

    else   
    {
        erase(begin() + n, end()); 
        std::fill(begin(), end(), value); 
    }
}

// copy assign 
template <typename T>  
template <typename InputIter>  
void deque<T>:: copy_assign(InputIter first, InputIter last, input_iterator_tag) 
{
    auto first1 = begin(); 
    auto last1 = end(); 
    for(; first1 != last1 && first != last; ++first, ++first1 )
    {
        *first1 =*first; 
    }
    // 没有使用完空间   
    if(first1 != last1) 
    {
        erase(first1, last1); 
    }
    else  // 使用完了空间 
    {
        insert_dispatch(_end, first, last, input_iterator_tag{}); 
    }
}

template <typename T> 
template <typename ForwardIter>  
void deque<T>:: copy_assign (ForwardIter first, ForwardIter last, forward_iterator_tag)
{
    const size_type len1 = size(); 
    const size_type len2 = mystl::distance(first, last); 
    if(len1 < len2) 
    {
        auto next =first; 
        mystl::advance(next, len1); 
        std::copy(first, next, _begin); 
        insert_dispatch(_end, next, last, forward_iterator_tag{}); 
    }
    else  
    {
        erase(std::copy(first, last, _begin), _end); 
    }
}

// insert_aux  
template <typename T> 
template <typename ...Args>  
typename deque<T>::iterator  
deque<T>::insert_aux(iterator pos, Args&& ... args) 
{
    const size_type elems_before = pos - _begin; 
    value_type value_copy = value_type(mystl::forward<Args>(args)...); 
    if(elems_before < size()/ 2) 
    {
        // insert at first half 
        emplace_front(front()); 
        auto front1 = _begin; 
        ++front1; 
        auto front2 = front1; 
        ++front2; 
        pos = _begin + elems_before; 
        auto pos1 = pos; 
        ++pos1; 
        std::copy(front2, pos, front1); 
    }

    else  
    {
        // insert at second half 
        emplace_back(back()); 
        auto back1 = end(); 
        --back1; 
        auto back2 = back1; 
        --back2; 
        pos = _begin + elems_before; 
        std::copy_backward(pos, back2, back1); 
    }
    *pos = mystl::move(value_copy); 
    return pos; 
} 

// fill_insert 
template<typename T>  
void deque<T>::fill_insert(iterator pos, size_type n, const value_type& value) 
{
    const size_type elems_before = pos - _begin; 
    const size_type len = size(); 
    auto value_copy = value; 
    if(elems_before < len / 2)
    {
        require_capacity(n, true); 
        // 原来的迭代器可能失效 
        auto old_begin = _begin; 
        auto new_begin = _begin - n; 
        pos = _begin + elems_before; 
        try
        {
            if(elems_before >= n) 
            {
                auto _begin_n = _begin + n; 
                std::uninitialized_copy(_begin, _begin_n, new_begin); 
                _begin = new_begin; 
                std::copy(_begin_n, pos, old_begin); 
                std::fill(pos - n, pos, value_copy); 
             }
            else  
            {
                std::uninitialized_fill(
                    std::uninitialized_copy(_begin, pos, new_begin), _begin, value_copy); 
                _begin = new_begin; 
                std::fill(old_begin, pos, value_copy); 
            }
        }
        catch(...)
        {
            if(new_begin.node != _begin.node) 
            { destroy_buffer(new_begin.node, _begin.node - 1); }
            throw; 
        } 
    }
    else  
    {
        require_capacity(n, false); 
        // 由于空间重新分配原来的迭代器可能会失效
        auto old_end = _end;
        auto new_end = _end + n; 
        const size_type elems_after = len - elems_before;
        pos = _end - elems_after; 
        try
        {
            if(elems_after > n) 
            {
                auto _end_n = _end - n; 
                std::uninitialized_copy(_end_n, _end, _end); 
                _end = new_end; 
                std::copy_backward(pos, _end_n, old_end); 
                std::fill(pos, pos + n, value_copy); 
            }
            else  
            {
                std::uninitialized_fill(_end, pos +n, value_copy); 
                std::uninitialized_copy(pos, _end, pos + n); 
                _end - new_end; 
                std::fill(pos, old_end, value_copy); 
            }
        }
        catch(...)
        {
           if(new_end.node != _end.node)
           { destroy_buffer(_end.node + 1, new_end.node); }
           throw; 
        }
    }
}

// copy insert 
template <typename T>  
template <typename ForwardIter>  
void deque<T>::copy_insert(iterator pos, ForwardIter first, ForwardIter last, size_type n) 
{
    const size_type elems_before = pos - _begin; 
    auto len = size(); 
    if(elems_before < len / 2) 
    {
        require_capacity(n, true); 
        // 原来迭代器可能会失效
        auto old_begin = _begin; 
        auto new_begin = _begin - n; 
        pos = _begin + elems_before; 

        try
        {
            if(elems_before >= n) 
            {
                auto _begin_n = _begin + n; 
                std::uninitialized_copy(_begin, _begin + n, new_begin); 
                _begin = new_begin; 
                std::copy(_begin_n, pos, old_begin); 
                std::copy(first, last, pos - n); 
            }
            else  
            {
                auto mid = first; 
                mystl::advance(mid, n - elems_before); 
                std::uninitialized_copy(first, mid, std::uninitialized_copy
                (_begin, pos, new_begin)); 
                _begin = new_begin; 
                std::copy(mid, last, old_begin); 
            }
        }
        catch(...)
        {
            if(new_begin.node != _begin.node) 
            {
                destroy_buffer(new_begin.node, _begin.node - 1); 
            }
            throw; 
        }
    }
    else  
    {
        require_capacity(n, false); 
        auto old_end = _end; 
        auto new_end = _end + n; 
        const auto elems_after = len - elems_before; 
        pos = _end - elems_after; 

        try
        {
            if(elems_after > n) 
            {
                auto _end_n = _end - n; 
                std::uninitialized_copy(_end_n, _end, _end); 
                _end = new_end; 
                std::copy_backward(pos, _end_n, old_end); 
                std::copy(first, last, pos); 
            }
            else  
            {
                auto mid = first; 
                mystl::advance(mid, elems_after); 
                std::uninitialized_copy(pos, _end, 
                std::uninitialized_copy(mid, last, _end)); 
                _end = new_end; 
                std::copy(first, mid, pos); 
            }
        }
        catch(...)
        {
            if(new_end.node != _end.node)
            { destroy_buffer(_end.node + 1, new_end.node); }
            throw; 
        }
    }
}

// insert_dispatch 
template <typename T>  
template <typename Iter>  
void deque<T>::  
insert_dispatch(iterator pos, Iter first, Iter last, input_iterator_tag)
{
    if(last <= first ) return; 
    const size_type n = mystl::distance(first, last); 
    const size_type elems_before = pos - _begin; 

    if(elems_before < size() / 2) 
    {
        require_capacity(n, true); 
    }
    else   
    {
        require_capacity(n, false); 
    }
    pos = _begin + elems_before; 

    auto cur = --last; 
    for(size_type i = 0; i < n; ++i, --cur)
    {
        insert(pos, *cur); 
    }
}

template <typename T>  
template <typename Iter>  
void deque<T>::  
insert_dispatch(iterator pos, Iter first, Iter last, forward_iterator_tag) 
{
    if(last <= first) return; 
    const size_type n = mystl::distance(first, last);  
    if(pos.cur == _begin.cur) 
    {
        require_capacity(n, true); 
        auto new_begin = _begin - n; 
        try
        {
            std::uninitialized_copy(first, last, new_begin); 
            _begin = new_begin;  
        }
        catch(...)
        {
            if(new_begin.node != _begin.node) 
                destroy_buffer(new_begin.node, _begin.node - 1);
            throw;  
        }
    }
    else if(pos.cur == _end.cur)   
    {
        require_capacity(n, false); 
        auto new_end = _end + n; 
        try
        {
            std::uninitialized_copy(first, last, _end); 
            _end = new_end; 
        }
        catch(...)
        {
            if(new_end.node != _end.node) 
                destroy_buffer(_end.node + 1, new_end.node); 
            throw; 
        }
    }
    else  
    {
        copy_insert(pos, first, last, n); 
    }
}

// require_capacity 
template <typename T>
void deque<T>:: require_capacity (size_type n, bool isFront)  
{
    if(isFront && (static_cast<size_type>(_begin.cur - _begin.first) < n)) 
    {
        const size_type need_buffer = (n - (_begin.cur - _begin.first)) / buffer_size + 1; 
        if(need_buffer > static_cast<size_type>(_begin.node - _map)) 
        {
            reallocate_map_at_front(need_buffer); 
            return; 
        }
        create_buffer(_begin.node - need_buffer, _begin.node - 1); 
    }
    else if(!isFront && (static_cast<size_type>(_end.last - _end.cur - 1) < n))
    {
        const size_type need_buffer = (n - (_end.last - _end.cur - 1)) / buffer_size + 1; 
        if(need_buffer > static_cast<size_type>((_map + _map_size) - _end.node - 1))
        {
            reallocate_map_at_back(need_buffer); 
            return; 
        }
        create_buffer(_end.node + 1, _end.node + need_buffer); 
    }
}

// reallocate_map_at_front 
template <typename T>  
void deque<T>::reallocate_map_at_front(size_type need_buffer)
{
    const size_type new_map_size = std::max(_map_size * 2, 
        _map_size + need_buffer); 

    map_pointer new_map = create_map(new_map_size); 
    const size_type old_buffer = _end.node - _begin.node + 1; 
    const size_type new_buffer = old_buffer + need_buffer; 

    // 将新的map 中的指针指向原来的buffer, 并且扩展buffer
    auto begin = new_map + (new_map_size - new_buffer) / 2; 
    auto mid = begin + need_buffer; 
    auto end = mid + old_buffer; 
    create_buffer(begin, mid - 1); 
    for(auto begin1 = mid, begin2 = _begin.node; begin1 != end; ++begin1, ++begin2)
    {
        *begin1 = *begin2; 
    }

    // update data 
    map_allocator::deallocate(_map, _map_size); 
    _map = new_map;
    _map_size = new_map_size;
    _begin = iterator(*mid + (_begin.cur - _begin.first), mid); 
    _end = iterator(*(end - 1) + (_end.cur - _end.first), end - 1); 


}




// reallocate_map_at_back
template<typename T>  
void deque<T>::reallocate_map_at_back(size_type need_buffer) 
{
    const size_type new_map_size = 
        std::max(_map_size *2, _map_size + need_buffer + DEQUE_MAP_INIT_SIZE); 
    map_pointer new_map = create_map(new_map_size); 
    const size_type old_buffer = _end.node - _begin.node + 1; 
    const size_type new_buffer = old_buffer + need_buffer; 

    // 新的map中指针指向原来buffer，并创建新的buffer 
    auto begin = new_map + ((new_map_size - new_buffer)/2); 
    auto mid = begin + old_buffer; 
    auto end = mid + need_buffer; 
    for(auto begin1 = begin, begin2 = _begin.node; begin1 != mid; ++begin1, ++begin2) 
    {
        *begin1 = *begin2;  
    }
    create_buffer(mid, end - 1); 

    // update data  
    map_allocator::deallocate(_map, _map_size); 
    _map = new_map; 
    _map_size = new_map_size; 
    _begin = iterator(*begin +(_begin.cur - _begin.first), begin); 
    _end = iterator(*(mid - 1) + (_end.cur - _end.first), mid - 1); 
}

// overloading relational operators  
template <typename T> 
bool operator== (const deque<T>& lhs, const deque<T>& rhs)
{
    return lhs.size() == rhs.size() &&  
        std::equal(lhs.begin(), lhs.end(), rhs.begin()); 
}

template <typename T>  
bool operator< (const deque<T>& lhs, const deque<T>& rhs) 
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.end()); 
}

template<typename T>  
bool operator!=(const deque<T>& lhs, const deque<T>& rhs) 
{
    return !(lhs == rhs); 
}

template<typename T>  
bool operator>(const deque<T>& lhs, const deque<T>& rhs) 
{
    return rhs < lhs; 
}

template <typename T> 
bool operator<=(const deque<T>& lhs, const deque<T>&rhs)
{
    return !(rhs < lhs); 
}

template <typename T>  
bool operator>=(const deque<T>& lhs, const deque<T>& rhs) 
{
    return !(lhs < rhs); 
}

// overloading generic swap  
template <typename T> 
void swap(deque<T>& lhs, deque<T>& rhs) 
{
    lhs.swap(rhs); 
}

} // end of mystl 
#endif // !_DEQUE_H_