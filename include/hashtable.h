#ifndef MYSTL_HASHTABLE_H_ 
#define MYSTL_HASHTABLE_H_ 


// template hashtable 
// 使用开链法来解决哈希冲突

#include <initializer_list>  
#include <algorithm> 
#include "functional.h"
#include "memory.h"
#include "vector.h"
#include "util.h"
#include "exceptdef.h"



namespace mystl
{

// hashtable node 
template <typename T> 
struct hashtable_node
{
    hashtable_node*     next;       
    T                   value; 

    //ctor
    hashtable_node() = default; 
    hashtable_node(const T& n ):next(nullptr), value(n) {} 

    hashtable_node(const hashtable_node& node ): next(node.next), value(node.value) {} 
    hashtable_node(hashtable_node&& node): next(node.next), value(node.value) 
    {
        node.next = nullptr; 
    }
}; 

// value traits 
template <typename T, bool> 
struct ht_value_traits_imp
{
    typedef T       key_type; 
    typedef T       mapped_type; 
    typedef T       value_type; 

    template <typename Ty> 
    static const key_type& get_key(const Ty& value)
    {
        return value; 
    }

    template <typename Ty>   
    static const value_type& get_value(const Ty& value)
    {
        return value; 
    }
}; 

template <typename T> 
struct ht_value_traits_imp<T, true>  
{
    typedef typename std::remove_cv<typename T::first_type>::type   key_type; 
    typedef typename T::second_type                                 mapped_type; 
    typedef T                                                       value_type; 

    template <typename Ty> 
    static const key_type& get_key(const Ty& value)
    {
        return value.first; 
    }

    template <typename Ty>   
    static const value_type& get_value(const Ty& value)
    {
        return value; 
    }
}; 

template <typename T> 
struct ht_value_traits 
{
    static constexpr bool is_map =  mystl::is_pair<T>::value; 

    typedef ht_value_traits_imp<T, is_map> value_traits_type;

    typedef typename value_traits_type::key_type                    key_type; 
    typedef typename value_traits_type::mapped_type                 mapped_type;
    typedef typename value_traits_type::value_type                  value_type;   

    template <typename Ty> 
    static const key_type& get_key(const Ty& value)
    {
        return value_traits_type::get_key(value); 
    }

    template <typename Ty>  
    static const value_type& get_value(const Ty& value)
    {
        return value_traits_type::get_value(value); 
    }
};

// forward declaration 
template <typename T, typename HashFun, typename KeyEqual> 
class hashtable; 

template <typename T, typename HashFun, typename KeyEqual> 
struct ht_iterator; 

template <typename T, typename HashFun, typename KeyEqual> 
struct ht_const_iterator; 

template <typename T> 
struct ht_local_iterator; 

template <typename T> 
struct ht_const_local_iterator; 

// hashtable iterator base 
template <typename T, typename Hash, typename KeyEqual> 
struct ht_iterator_base: public mystl::iterator<mystl::forward_iterator_tag, T> 
{
    typedef mystl::hashtable<T, Hash, KeyEqual>         hashtable; 
    typedef ht_iterator_base<T, Hash, KeyEqual>         base; 
    typedef mystl::ht_iterator<T, Hash, KeyEqual>       iterator; 
    typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator; 
    typedef hashtable_node<T*>                          node_ptr; 
    typedef hashtable*                                  contain_ptr; 
    typedef const node_ptr                              const_node_ptr; 
    typedef const contain_ptr                           const_contain_ptr; 

    typedef size_t                                      size_type; 
    typedef ptrdiff_t                                   difference_type; 

    node_ptr        node;       // current node 
    contain_ptr     ht;         // attachment with container 

    ht_iterator_base() = default; 

    bool operator == (const base& rhs) const { return node == rhs.node; } 
    bool operator != (const base& rhs) const { return node != rhs.node; }
}; 

template <typename T, typename Hash, typename KeyEqual> 
struct ht_iterator: public ht_iterator_base<T, Hash, KeyEqual> 
{
    // traits 
    typedef ht_iterator_base<T, Hash, KeyEqual>             base;
    typedef typename base::hashtable                        hashtable; 
    typedef typename base::iterator                         iterator; 
    typedef typename base::const_iterator                   const_iterator; 
    typedef typename base::node_ptr                         node_ptr; 
    typedef typename base::contain_ptr                      contain_ptr; 

    typedef ht_value_traits<T>                              value_traits; 
    typedef T                                               value_type; 
    typedef value_type*                                     pointer; 
    typedef value_type&                                     reference; 

    using base::node; 
    using base::ht; 

    //ctor 
    ht_iterator() = default; 
    // 不能从初始化列表初始化基类成员
    ht_iterator(node_ptr n, contain_ptr t)
    {
        node = n; 
        ht = t; 
    }

    ht_iterator(const iterator& rhs)
    {
        node = rhs.node;
        ht = rhs.ht; 
    }

    ht_iterator(const const_iterator rhs)
    {
        node = rhs.node; 
        ht = rhs.ht; 
    }

    iterator& operator= (const iterator& rhs)
    {
        if(this != &rhs)
        {
            node = rhs.node; 
            ht = rhs.ht;
        }
        return *this; 
    }

    iterator& operator= (const const_iterator& rhs)
    {
        if(this != &rhs)
        {
            node = rhs.node; 
            ht = rhs.ht; 
        }
        return *this; 
    }

    // operator overloading 
    reference operator* () const { return node->value; } 
    pointer   operator->() const { return &(operator*()); }

    iterator& operator++() 
    {
        MYSTL_DEBUG(node != nullptr); 
        const node_ptr old = node; 
        if(nullptr == node)
        {
            // 如果到达末尾，则跳转到下一个合法的node处
            auto index = ht->hash(value_traits::get_key(old->value)); 
            while(!node && ++index < ht->bucket_size_)
                node = ht->buckets_[index]; 
        }
        return *this; 
    }

    iterator operator++(int)
    {
        iterator temp = *this; 
        ++*this; 
        return temp; 
    }
}; 

template <typename T, typename Hash, typename KeyEqual> 
struct ht_const_iterator: public ht_iterator_base<T, Hash, KeyEqual> 
{
    typedef ht_iterator_base<T, Hash, KeyEqual>             base; 
    typedef typename base::hashtable                        hashtable; 
    typedef typename base::iterator                         iterator; 
    typedef typename base::const_iterator                   const_iterator; 
    typedef typename base::const_node_ptr                   node_ptr; 
    typedef typename base::const_contain_ptr                contain_ptr; 

    typedef ht_value_traits<T>                              value_traits; 
    typedef T                                               value_type; 
    typedef const value_type*                               pointer; 
    typedef const value_type&                               reference; 

    using base::node; 
    using base::ht; 
    
    ht_const_iterator() = default; 
    ht_const_iterator(node_ptr n, contain_ptr t)
    {
        node = n; 
        ht = t; 
    } 

    ht_const_iterator(const iterator& rhs)
    {
        node = rhs.node; 
        ht = rhs.ht; 
    }

    ht_const_iterator(const const_iterator& rhs)
    {
        node = rhs.node; 
        ht = rhs.ht; 
    }

    const_iterator& operator= (const iterator& rhs)
    {
        if(this != &rhs)
        {
            node = rhs.node; 
            ht = rhs.ht; 
        }
        return *this; 
    }

    const_iterator& operator= (const const_iterator& rhs)
    {
        if(this != &rhs)
        {
            node = rhs.node; 
            ht = rhs.ht; 
        }
        return *this; 
    }

    // opeartor overloading 
    reference operator*() const { return node->value; } 
    pointer   operator->() const { return &(operator*()); } 

    const_iterator& operator++() 
    {
        MYSTL_DEBUG(nullptr != node); 
        const node_ptr old = node; 
        node = node -> next; 
        if(nullptr == node)
        {
            auto index = ht->hash(value_traits::get_key(old->value)); 
            while(!node && index < ht->bucket_size_)
            {
                node = ht->buckets_[index]; 
            }
        }
        return *this; 
    }

    const_iterator operator++(int)
    {
        const_iterator temp = *this; 
        ++(*this); 
        return temp; 
    }


}; 



}   // end of namespace mystl 
#endif 

