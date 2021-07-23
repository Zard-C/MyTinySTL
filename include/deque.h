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
#include "iterator.h"
#include "memory.h"
#include "util.h" 
#include "exceptdef.h" 

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
    value_pointer   node;           // 缓冲区所在节点

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
            cur - first; 
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
    {fill_init(0, value_type()); }

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
            // 这里怎么释放的? 
            data_allocator::deallocate(*_begin.node, buffer_size);
            _begin->node = nullptr; 
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
    void emplace(Args&& ... args); 

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
    
}; 



} // end of mystl 
#endif // !_DEQUE_H_