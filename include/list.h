// list.h -- 模版类链表
#ifndef _LIST_H_
#define _LIST_H_

// 异常保证
//  mystl::list<T> 满足基本的异常保证，部分函数无异常保证，并对以下函数做强异常
// 安全保证
// * emplace_front
// * emplate_back 
// * emplace
// * push_front 
// * push_back
// * insert 

#include <initializer_list> 
#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"


namespace mystl 
{
template <typename T> 
struct list_node_base; 

template <typename T> 
struct list_node; 


template <typename T> 
struct node_triats 
{
    typedef list_node_base<T>*      base_ptr; 
    typedef list_node<T>*           node_ptr; 
};

// list_node_base 
template <typename T> 
struct list_node_base
{
    typedef typename node_triats<T>::base_ptr       base_ptr; 
    typedef typename node_triats<T>::node_ptr       node_ptr; 

    base_ptr    prev; 
    base_ptr    next; 

    // ctor 
    list_node_base() = default; 

    node_ptr as_node() 
    {
        return static_cast<node_ptr>(self()); 
    }

    void unlink()
    {
        prev = next = self(); 
    }

    base_ptr self() 
    {
        return static_cast<base_ptr>(&*this); 
    }
}; 

template<typename T> 
struct list_node: public list_node_base <T> 
{
    typedef typename node_triats<T>::base_ptr       base_ptr; 
    typedef typename node_triats<T>::node_ptr       node_ptr; 

    T value; 

    // default ctor 
    list_node() = default; 

    list_node(const T& val): value(val) {} 

    // move ctor 
    list_node(T&& val): value(mystl::move(val)) {} 

    base_ptr as_base() 
    {
        return static_cast<base_ptr>(&*this); 
    }

    node_ptr self()
    {
        return static_cast<node_ptr>(&*this); 
    }
};

// iterator 
template <typename T> 
struct list_iterator :public mystl::iterator<mystl::bidirectional_iterator_tag, T> 
{
    typedef T                                       value_type; 
    typedef T*                                      pointer; 
    typedef T&                                      reference; 
    typedef typename node_triats<T>::base_ptr       base_ptr;
    typedef typename node_triats<T>::node_ptr       node_ptr; 
    typedef list_iterator<T>const                   self; 

    base_ptr        _node;  // 指向当前节点

    // ctors 
    list_iterator() = default; 
    list_iterator(base_ptr x): _node(x) {}; 
    list_iterator(node_ptr x): _node(x->as_base()) {} 
    list_iterator(const list_iterator& rhs): _node(rhs._node) {}  

    // overload operators 
    reference  operator* () const {return _node->as_node()->value; }
    pointer    operator& () const {return &(operator*()); }

    // 后置++ 
    self& operator++()
    {
        MYSTL_DEBUG(nullptr != _node); 
        _node = _node->next; 
        return *this; 
    }

    // 前置++ 
    self& operator++(int)
    {   
        MYSTL_DEBUG(nullptr != _node); 
        self temp = *this; 
        _node = _node->next; 
        return temp; 
    }

    // 前置-- 
    self& operator--()
    {
        MYSTL_DEBUG(nullptr != _node); 
        _node = _node->prev; 
        return *this; 
    }

    // 后置-- 
    self& operator--(int)
    {
        MYSTL_DEBUG(nullptr != _node); 
        self temp = *this; 
        --*this; 
        return temp; 
    }

    // relational operators 
    bool operator == (const self& rhs) const {return _node == rhs._node; }
    bool operator != (const self& rhs) const {return _node != rhs._node; }

}; 

// const_iterator 
template <typename T>  
struct list_const_iterator: public mystl::iterator<mystl::bidirectional_iterator_tag, T> 
{
    typedef T                                       value_type; 
    typedef const T*                                pointer; 
    typedef const T&                                reference;
    typedef typename node_triats<T>::base_ptr       base_ptr; 
    typedef typename node_triats<T>::node_ptr       node_ptr; 
    typedef list_const_iterator<T>                  self; 

    base_ptr        _node; 

    // ctors 
    list_const_iterator() = default; 
    list_const_iterator(base_ptr x): _node(x) {} 
    list_const_iterator(node_ptr x): _node(x->as_base()) {} 
    list_const_iterator(const list_iterator<T>& rhs): _node(rhs._node) {} 
    list_const_iterator(const list_const_iterator<T>& rhs): _node(rhs._node) {} 

    // overload operators 
    reference operator*() const {return _node->as_node()->value; }
    pointer   operator->() const {return &(operator*()); }

    // 
    self& operator++()
    {
        MYSTL_DEBUG(nullptr != _node); 
        _node = _node->next; 
        return *this; 
    }

    self& operator++(int)
    {
        MYSTL_DEBUG(nullptr != _node);
        self temp = *this; 
        ++(*this);
        return temp; 
    }

    self& operator--()
    {
        MYSTL_DEBUG(nullptr != _node); 
        _node = _node->prev; 
        return *this; 
    }

    self& operator--(int)
    {
        MYSTL_DEBUG(nullptr != _node); 
        self temp = *this; 
        --(*this); 
        return temp; 
    }

    // relational operator 
    bool operator== (const self& rhs) const {return _node == rhs._node; }
    bool operator!= (const self& rhs) const {return _node != rhs._node; }

};

// list 
template <typename T> 
class list 
{
public: 
    typedef mystl::allocator<T>                         allocator_type; 
    typedef mystl::allocator<T>                         data_allocator;  
    typedef mystl::allocator<list_node_base<T>>         base_allocator; 
    typedef mystl::allocator<list_node<T>>              node_allocator; 

    typedef typename allocator_type::value_type         value_type; 
    typedef typename allocator_type::pointer            pointer; 
    typedef typename allocator_type::const_pointer      const_pointer; 
    typedef typename allocator_type::reference          reference; 
    typedef typename allocator_type::const_reference    const_reference; 
    typedef typename allocator_type::size_type          size_type; 
    typedef typename allocator_type::difference_type    difference_type; 

    typedef list_iterator<T>                            iterator; 
    typedef list_const_iterator<T>                      const_iterator; 
    typedef mystl::reverse_iterator<iterator>           reverse_iterator; 
    typedef mystl::reverse_iterator<const_iterator>     const_reverse_iterator; 

    typedef typename node_triats<T>::base_ptr           base_ptr; 
    typedef typename node_triats<T>::node_ptr           node_ptr; 

    allocator_type  get_allocator() {return node_allocator(); }

private:   
    base_ptr    _node;      // 指向末尾节点 
    size_type   _size;      // 大小


public:  
    // ctor 
    // fill 
    list() 
    {
        fill_init(0, value_type()); 
    }
    
    explicit list(size_type n)
    {
        fill_init(n, value_type()); 
    }

    list(size_type n, const T& value)
    {
        fill_init(n, value); 
    }

    // range 
    template <typename Iter, 
        typename std::enable_if<mystl::is_input_iterator<T>::value,int>::type = 0>
    list(Iter first, Iter last)
    {
        copy_init(first, last); 
    }   

    list(std::initializer_list<T> initlist)
    {
        copy_init(initlist.begin(), initlist.end()); 
    }

    // copy ctor 
    list(const list& rhs)
    {
        copy_init(rhs.cbegin(), rhs.cend()); 
    }

    // move ctor 
    list(list&& rhs) noexcept: _node(rhs._node), _size(rhs._size)
    {
        rhs._node = nullptr; 
        rhs._size = 0; 
    }

    // copy assign 
    list& operator= (const list& rhs)
    {
        if(this !=rhs )  
        {
            assign(rhs.begin(), rhs.end()); 
        }
        return *this; 
    }

    // move assign 
    list& operator= (list &&rhs) noexcept 
    {
        // 清空自身节点
        clear(); 
        // 将rhs 接在end之前
        splice(end(), rhs); 
        return *this; 
    }

    // range copy assign 
    list& operator= (std::initializer_list<T> initlist)
    {
        list temp (initlist.begin(), initlist.end()); 
        swap(temp);
        return *this; 
    }

    // dtor 
    ~list() 
    {
        if(_node)
        {
            clear(); 
            base_allocator::deallocate(_node); 
            _node = nullptr; 
            _size = 0; 
        }
    }

public:  

    // 迭代器ops begin && end 
    iterator            begin()         noexcept 
    {return _node->next; }
    const_iterator      begin()         const noexcept 
    {return _node->next;} 
    iterator            end()           noexcept 
    {return _node; }
    const_iterator      end()           const noexcept 
    {return _node;}

    // rbegin && rend 
    reverse_iterator   rbegin()         noexcept 
    {return reverse_iterator(end()); }
    const_reverse_iterator rbegin()     const noexcept 
    {return reverse_iterator(end()); }
    reverse_iterator   rend()           noexcept 
    {return reverse_iterator(begin()); }
    reverse_iterator   rend()           const noexcept 
    {return reverse_iterator(begin()); }

    // cbegin && cend 
    const_iterator      cbegin()        const noexcept
    {return begin(); } 
    const_iterator      cend()          const noexcept 
    {return end(); } 
    const_reverse_iterator  crbegin()   const noexcept 
    {return rbegin(); } 
    const_reverse_iterator  crend()     const noexcept 
    {return rend(); } 

    // 容量相关ops 
    bool empty()        const noexcept
    {
        return _node->next == _node; 
    }

    size_type size()    const noexcept 
    {
        return _size; 
    }

    size_type max_size() const noexcept 
    {
        return static_cast<size_type>(-1); 
    }

    // 元素访问ops
    reference   front() 
    {
        MYSTL_DEBUG(!empty()); 
        return *begin(); 
    }

    const_reference front() const 
    {
        MYSTL_DEBUG(!empty()); 
        return *begin(); 
    }

    reference   back() 
    {
        MYSTL_DEBUG(!empty()); 
        return *(--end()); 
    }

    const_reference back()  const 
    {
        MYSTL_DEBUG(!empty()); 
        return *(--end());
    }

    // 容器调整相关操作

    // assign 
    void assign (size_type n, const value_type& value)
    { fill_assign(n, value); }

    template<typename Iter, 
    typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type = 0> 
    void assign(Iter first, Iter last)
    {
        copy_assign(first, last); 
    }

    void assign(std::initializer_list<T> initlist)
    {
        copy_assign(initlist.begin(), initlist.end()); 
    }

    //emplace / emplace_front / emplace_back 
    template <typename... Args>   
    void emplace_front(Args&& ...args)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() -1, "list<T> is too big"); 
        auto link_node = create_node(mystl::forward<Args>(args)...); 
        link_nodes_at_front(link_node->as_base(), link_node->as_base());
        ++_size; 
    }
    

}; 


} // end of namespace mystl 
#endif // !_LIST_H_