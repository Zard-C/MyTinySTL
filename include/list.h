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

    template <typename ...Args>  
    void emplace_back(Args&&... args)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T> is too big"); 
        auto link_node = create_node(mystl::forward<Args>(args)...); 
        link_nodes_at_back(link_node->as_base(), link_node->as_base()); 
        ++_size; 
    }

    template <typename ...Args>   
    iterator emplace(const_iterator pos, Args&& ...args)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T> is too big"); 
        auto link_node = create_node(mystl::forward<Args>(args)...); 
        link_nodes(pos._node, link_node->as_base(), link_node->as_base()); 
        ++_size; 
        return iterator(link_node); 
    }

    // insert 
    iterator insert(const_iterator pos, const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T>'s size too big"); 
        auto link_node = create_node(value); 
        ++_size; 
        return link_iter_node(pos, link_node->as_base()); 
    }

    iterator insert(const_iterator pos, value_type&& value)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T>'s size too big"); 
        auto link_node = create_node(mystl::move(value)); 
        ++_size; 
        return link_iter_node(pos, link_node->as_base()); 
    }

    // insert n 
    iterator insert(const_iterator pos, size_type n, const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - n, "list<T>'s size too big");
        return fill_insert(pos, n, value); 
    }

    // range insert 
    template <typename Iter, 
    typename std::enable_if<mystl::is_input_iterator<Iter>::value, int> = 0> 
    iterator insert(const_iterator pos, Iter first, Iter last)
    {
        size_type n = mystl::distance(first, last); 
        THROW_LENGTH_ERROR_IF(_size > max_size() - n, "list<T>'s size too big"); 
        return copy_insert(pos, n, first); 
    }

    // push_front / push_back 
    void push_front(const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T>'s size too big"); 
        auto link_node = create_node(value); 
        link_nodes_at_front(link_node->as_base(), link_node->as_base()); 
        ++_size; 
    }

    void push_front(value_type&& value)
    {
        emplace_front(mystl::move(value)); 
    }

    void push_back(const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - 1, "list<T>'s size too big"); 
        auto link_node = create_node(value); 
        link_nodes_at_back(link_node->as_base(), link_node->as_base()); 
        ++_size; 
    }

    void push_back(value_type&& value)
    {
        emplace_back(mystl::move(value)); 
    }

    // pop_front / pop_back 
    void pop_front() 
    {
        MYSTL_DEBUG(!empty()); 
        auto n = _node->next; 
        unlink_nodes(n, n); 
        destroy_nodes(n->as_node()); 
        --_size; 
    }

    void pop_back()
    {
        MYSTL_DEBUG(!empty()); 
        auto n = _node->prev; 
        unlink_nodes(n, n); 
        destroy_node(n->as_node());
        --_size; 
    }

    // erase / clear 
    iterator erase(const_iterator pos); 
    iterator erase(const_iterator first, const_iterator last); 

    void clear(); 

    // resize 
    void resize(size_type new_size) {resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type& value); 

    // swap 
    void swap(list& rhs) noexcept 
    {
        if(this != &rhs)
        {
            mystl::swap(_node, rhs._node); 
            mystl::swap(_size, rhs._size); 
        }
    }

    // list operations 
    void splice(const_iterator pos, list& other); 
    void splice(const_iterator pos, list& other, const_iterator it); 
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last); 

    // remove 
    void remove(const value_type& value)
    {remove_if([&](const value_type& v){return v == value; }); } 
    template <typename UnaryPredicate>   
    void remove_if(UnaryPredicate pred); 

    // unique
    void unique()
    { unique(mystl::equal_to<T>()); }

    template <typename BinaryPredicate>   
    void unique(BinaryPredicate pred); 

    // merge 
    void merge(list& x)
    {
        merge(x, mystl::less<T>()); 
    }
    template <typename Compare>   
    void merge(list& x, Compare comp); 

    // sort 
    void sort()
    {list_sort(begin(), end(), size(), mystl::less<T>()); } 
    template <typename Compare>   
    void sort(Compare comp)
    {list_sort(begin(), end(), size(), comp); }

    // reverse 
    void reverse(); 

private:  
    // auxiliary methods 

    // create / destroy node 
    template <typename ...Args>   
    node_ptr create_node(Args&& ...args); 

    void destroy_node(node_ptr p); 

    // initialize 
    void fill_init(size_type n, const value_type& value); 
    template <typename Iter>   
    void copy_init(Iter first, Iter last);

    // link / unlink 
    iterator link_iter_node(const_iterator pos, base_ptr node); 
    void link_nodes(base_ptr p, base_ptr first, base_ptr last); 
    void link_nodes_at_front(base_ptr first, base_ptr last); 
    void link_nodes_at_back(base_ptr first, base_ptr last); 
    void unlink_nodes(base_ptr first, base_ptr last); 

    // assign 
    void fill_assign(size_type n, const value_type& value); 
    template <typename Iter>   
    void copy_assign(Iter first, Iter last); 

    // insert 
    iterator fill_insert(const_iterator pos, size_type n, const value_type& value);  
    template <typename Iter>   
    iterator copy_insert(const_iterator pos, size_type n, Iter first); 

    // sort 
    template <typename Compare>  
    iterator list_sort(iterator first, iterator last, size_type n, Compare comp); 
};  // end of class list<T> 

// erase pos 
template <typename T> 
typename list<T>::iterator 
list<T>::erase(const_iterator pos) 
{
    MYSTL_DEBUG(pos != end()); 
    auto n = pos._node; 
    auto next = n->next; 
    unlink_nodes(n, n); 
    destroy_node(n->as_node()); 
    --_size; 
    return iterator(next); 
}

// erase [first, last) 
template<typename T> 
typename list<T>::iterator 
list<T>::erase(const_iterator first, const_iterator last)
{
    if(first != last) 
    {
        unlink_nodes(first._node, last._node->prev);
        while(first != last) 
        {
            auto cur = first._node; 
            ++first; 
            destroy_node(cur->as_node()); 
            --_size; 
        } 
    }
    return iterator(last._node); 
}

// clear list 
template <typename T> 
void list<T>::clear() 
{
    if(_size != 0)
    {
        auto cur = _node->next; 
        for(base_ptr next = cur->next; cur != _node; cur = next, next = cur->next)
        {
            destroy_node(cur->as_node()); 
        }
        _node->unlink(); 
        _size = 0; 
    }
}

// resize 
template <typename T> 
void list<T>::resize(size_type new_size, const value_type& value)
{
    auto ite = begin(); 
    size_type len = 0; 
    while(ite != end() && len < new_size)
    {
        ++ite; 
        ++len; 
    }

    if(len == new_size)
    {
        erase(ite, _node); 
    }
    else  
    {
        insert(_node, new_size - len, value); 
    }
}

// splice: 将list x 接在pos之前
template <typename T> 
void list<T>::splice(const_iterator pos, list& x) 
{
    MYSTL_DEBUG(this != &x); 
    if(!x.empty())
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - x.size(),  "list<T>'s size too big"); 
        
        auto first = x._node->next; 
        auto last = x._node->prev; 

        x.unlink_nodes(first, last); 

        link_nodes(pos._node, first, last); 
        _size += x._size; 
        x._size = 0; 
    }
}

// splice: 将it 指向节点接在pos之前
template <typename T>   
void list<T>::splice(const_iterator pos, list& x, const_iterator it) 
{
    if(pos._node != it._node && pos._node != it._node->next)
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() -1, "list<T>'s size too big"); 
    }

    auto first = it._node; 
    x.unlink_nodes(first, first); 
    link_nodes(pos._node, first, first); 

    ++_size; 
    --x._size; 
}

// splice: list x 的[first, last) 节点接在pos之前
template <typename T> 
void list<T>:: splice(const_iterator pos, list& x, const_iterator first, const_iterator last) 
{
    if(first != last && this != &x) 
    {
        size_type n = mystl::distance(first, last); 
        THROW_LENGTH_ERROR_IF(_size > max_size() - n, "list<T>'s size too big"); 
        auto f = first._node; 
        auto l = last._node; 

        x.unlink(f, l); 
        link_nodes(pos.node, f, l); 

        _size +=n; 
        x._size -=n; 
    }

}

// remove_if: 删除满足条件的元素
template<typename T> 
template<typename UnaryPrdicate> 
void list<T>::remove_if(UnaryPrdicate pred)
{
    auto first = begin(); 
    auto last = end(); 
    for(auto next = first; first != last; first = next)
    {
        ++next; 
        if(pred(*first)) erase(first); 
    }
}

// remove_if:
template <typename T> 
template <typename BinaryPredicate> 
void list<T>::unique(BinaryPredicate pred) 
{
    auto first = begin(); 
    auto last = end(); 
    auto ite = first; 
    ++ite; 
    while(ite != last) 
    {
        if(pred(*first, *ite)) erase(ite); 
        else first = ite; 
        ite = first; 
        ++ite; 
    }
} 

// merge with another list 按照comp 为true 的顺序
template <typename T>   
template <typename Compare>  
void list<T>:: merge(list& x, Compare comp)
{
    if(this != &x) 
    {
        THROW_LENGTH_ERROR_IF(_size > max_size() - x._size, "list<T>'s size too big"); 

        auto f1 = begin(); 
        auto l1 = end(); 
        auto f2 = x.begin(); 
        auto l2 = x.end(); 

        while(f1 != l1 && f2 != l2)
        {
            if(comp(*f2, *f1))
            {
                auto next = f2; 
                ++next; 
                for(; next != l2 && comp(*next, *f1); ++next)
                {
                    ;
                }
                auto f = f2._node; 
                auto l = next._node->prev; 
                f2 = next; 

                // link_node 
                x.unlink_nodes(f, l); 
                link_nodes(f1._node, f, l);
                ++f1; 
            }
            else  
            {
                ++f1; 
            }
        }

        if(f2 != l2) 
        {
            auto f = f2._node; 
            auto l = l2._node->prev; 
            x.unlink_nodes(f, l);
            link_nodes(l1._node, f, l); 
        }

        _size += x._size; 
        x._size = 0; 
    }
}

// reverse list 
template <typename T> 
void list<T>::reverse()
{
    if(_size < 1) return; 

    auto first = begin();
    auto last = end(); 
    for(; first._node != last._node; first._node = first._node->next) 
    {
        mystl::swap(first._node->prev, first._node->next);
    }
    mystl::swap(last._node->prev, last._node->next); 
}

// helper function 

// create nodes 
template <typename T> 
template <typename...Args>   
typename list<T>::node_ptr 
list<T>::create_node(Args&&...args) 
{
    node_ptr p = node_allocator::allocate(1); 
    try
    {
        data_allocator::construct(mystl::address_of(p->value), mystl::forward<Args>(args)...);
        p->prev = nullptr;
        p->next = nullptr; 
    }
    catch(...)
    {
        node_allocator::deallocate(p);
        throw; 
    }
    return p; 
}

// destroy node 
template <typename T> 
void list<T>::destroy_node(node_ptr p) 
{
    data_allocator::destroy(mystl::address_of(p->value)); 
    node_allocator::deallocate(p); 
}

// fill_init 
template <typename T> 
void list<T>::fill_init(size_type n, const value_type& value)
{
    _node = base_allocator::allocate(1); 
    _node->unlink(); 

    _size = n; 

    try
    {
        for(; n > 0; --n)
        {
            auto node = create_node(value); 
            link_nodes_at_back(node->as_base(), node->as_base()); 
        }
    }
    catch(...)
    {
        clear(); 
        base_allocator::deallocate(_node); 
        _node = nullptr; 
        _size = 0; 
        throw; 
    }
}

// copy_init [first, last) 
template <typename T> 
template <typename Iter>  
void list<T>::copy_init(Iter first, Iter last) 
{
    _node = base_allocator::allocate(1);
    _node->unlink(); 

    size_type n = mystl::distance(first, last); 
    _size = n; 

    try
    {
        for(; n > 0; --n, ++first) 
        {
            auto node = create_node(*first); 
            link_nodes_at_back(node->as_base(), node->as_base()); 
        }
    }
    catch(...)
    {
        clear();
        base_allocator::deallocate(_node); 
        _node = nullptr; 
        _size = 0; 
        throw;  
    }
}

// pos 连接一个节点(pos -> link_node)
template<typename T> 
typename list<T>::iterator 
list<T>::link_iter_node(const_iterator pos, base_ptr link_node)
{
    if(pos == _node->next)
    {
        link_nodes_at_front(link_node, link_node); 
    }
    else if(pos == _node)
    {
        link_nodes_at_back(link_node, link_node); 
    }
    else   
    {

        link_nodes(pos._node, link_node, link_node); 
    }
    return iterator(link_node); 
}

// pos 处连接[first, last) [first, last]->pos
template <typename T> 
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last) 
{
    pos->prev->next = first; 
    first->prev = pos->prev; 
    pos->prev = last; 
    last->next = pos; 
}

// 头部连接 [first, last] 
template<typename T> 
void list<T>:: link_nodes_at_front(base_ptr first, base_ptr last) 
{
    first->prev =_node; 
    last->next = _node->next; 
    last->next->prev = last; 
    _node->next = first; 
}

// 尾部连接 [first, last] 
template<typename T>   
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last) 
{
    last->next = _node; 
    first->prev = _node->prev; 
    first->prev->next = first; 
    _node->prev = last;
}

// list 与其中的一段[first, last] 断开连接
template <typename T> 
void list<T>::unlink_nodes(base_ptr first, base_ptr last)
{
    first->prev->next = last->next; 
    last->next->prev = first->prev; 
}

// fill_assign n 
template<typename T> 
void list<T>::fill_assign(size_type n, const value_type& value) 
{
    auto first = begin(); 
    auto last = end(); 

    for(; n>0 && first != last; --n, ++first)
    {
        *first = value; 
    }
    if(n > 0) 
    {
        insert(last, n, value); 
    }
    else  
    {
        erase(first, last); 
    }
}

// copy_assign[f2, l2)  
template <typename T> 
template <typename Iter> 
void list<T>::copy_assign(Iter f2, Iter l2) 
{
    auto f1 = begin(); 
    auto l1 = end(); 
    for(; f1 != l1 && f2 != l2; ++f1, ++f2) 
    {
        *f1 = *f2; 
    }

    if(f2 == l2) 
    {
        erase(f1, l2); 
    }
    else  
    {
        insert(l1, f2, l2); 
    }
}

// pos fill_assign n 
template <typename T> 
typename list<T>::iterator 
list<T>::fill_insert(const_iterator pos, size_type n, const value_type& value) 
{
    iterator r(pos._node); 
    if(n != 0)
    {
        const auto add_size = n; 
        auto node = create_node(value); 
        node->prev= nullptr; 
        r = iterator(node); 
        iterator end = r; 

        try
        {
            for(--n; n > 0; --n, ++end)  
            {
                auto next = create_node(value); 
                end._node->next = next->as_base(); 
                next->prev = end._node; 
            }
            _size += add_size; 
        }
        catch(...)
        {
            auto enode = end._node; 
            while(true)
            {
                auto prev = enode->prev; 
                destroy_node(enode->as_node()); 
                if(nullptr == prev)
                {
                    break; 
                }
                enode = prev; 
            }
            throw; 
        }
        link_nodes(pos._node, r._node, end._node);
    }
    return r; 
}

// pos 处插入[first, last) 
template<typename T> 
template <typename Iter >  
typename list<T>::iterator 
list<T>::copy_insert(const_iterator pos, size_type n, Iter first) 
{
    iterator r(pos._node); 
    if(n != 0)
    {
        const auto add_size = n; 
        auto node = create_node(*first); 
        node->prev = nullptr; 
        r = iterator(node); 
        iterator end = r; 

        try
        {
            for(--n, ++first; n > 0; --n, ++first, ++end) 
            {
                auto next = create_node(*first); 
                end._node->next = next->as_base(); 
                next->prev = end._node; 
            }
            _size += add_size; 
        }
        catch(...)
        {
            auto enode = end._node; 
            while(true)
            {
                auto prev = enode->prev; 
                destroy_node(enode->as_node()); 
                if(nullptr == prev)
                {
                    break; 
                }
                enode = prev; 
            }
            _size -=add_size; 
            throw; 
        }
        link_nodes(pos._node, r._node, end._node); 
    }

    return r; 
}

// list merge sort  
template <typename T> 
template <typename Compare>   
typename list<T>::iterator 
list<T>:: list_sort(iterator f1, iterator l2, size_type n, Compare cmp)
{
    if(n < 2) return f1; 

    if(n == 2) 
    {
        if(cmp(*--l2, *f1))
        {
            auto ln = l2._node; 
            unlink_nodes(ln, ln); 
            link_nodes(f1._node, ln, ln);
            return l2; 
        }
        return f1; 
    }

    auto n2 = n/2; 
    auto l1 = f1; 
    mystl::advance(l1, n2); 
    auto result = f1 = list_sort(f1, l1, n2, cmp); 
    auto f2 =l1 =list_sort(l1, l2, n - n2, cmp); 

    if(cmp(*f2, *f1))
    {
        auto m = f2; 
        ++m; 
        for(; m !=l2 && cmp(*m, *f1); ++m)
        {
            ; 
        }

        auto f = f2._node; 
        auto l = m._node->prev; 
        result = f2; 
        l1 = f2 = m; 
        unlink_nodes(f, l); 
        m = f1; 
        ++m; 
        link_nodes(f1._node, f, l); 
        f1 = m; 
    }
    else  
    {
        ++f1; 
    }

    while(f1 != l1 && f2 != l2)
    {
        if(comp(*f2, *f1))
        {
            auto m = f2; 
            ++m; 
            for(; m != l2 && cmp(*m, *f1); ++m)
            {
                ; 
            }
            auto f = f2._node; 
            auto l = m._node->prev; 
            if(l1 == f2)
            {
                l1 = m; 
            }
            f2 = m; 
            unlink_nodes(f, 1); 
            m = f1; 
            ++m;
            link_nodes(f1._node, f, l); 
            f1 = m; 
        }
        else
        {
            ++f1; 
        }
    }
    return result; 
}

// overloads relational opeartors 
template <typename T> 
bool operator==(const list<T>& lhs, const list<T>& rhs)
{
    auto f1 = lhs.begin(); 
    auto l1 = lhs.end(); 
    auto f2 = rhs.begin(); 
    auto l2 = rhs.end(); 
    for(; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
    {
        ;
    }
    return f1 == l1 && f2 == l2; 
}

template <typename T> 
bool operator< (const list<T>& lhs, const list<T>& rhs)
{
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbefore_begin(), rhs.cend());
}

template <typename T> 
bool operator != (const list<T>& lhs, const list<T>& rhs)
{
    return !(lhs == rhs); 
}

template <typename T> 
bool operator >(const list<T>& lhs, const list<T>& rhs)
{
    return (rhs < lhs); 
}

template<typename T> 
bool operator <=(const list<T>& lhs, const list<T>& rhs)
{
    return !(rhs < lhs);
}

template <typename T>   
bool operator>= (const list<T>& lhs, const list<T>& rhs)
{
    return !(lhs < rhs); 
}

// generic swap 
template<typename T> 
void swap(list<T>& lhs, list<T>& rhs)
{
    lhs.swap(rhs);
}

} // end of namespace mystl 
#endif // !_LIST_H_