#ifndef  RBTREE_H_
#define  RBTREE_H_

// 模版类:rbtree 
#include <initializer_list> 
#include <cassert> 
#include <functional> 
#include "iterator.h" 
#include "memory.h" 
#include "type_traits.h"
#include "exceptdef.h"
 
namespace mystl 
{

// rbtree nodes color 
typedef bool rb_tree_color_type; 
static constexpr rb_tree_color_type rb_tree_red = false; 
static constexpr rb_tree_color_type rb_tree_black = true; 
 
// forward declaration 
template <typename T> struct rb_tree_node_base; 
template <typename T> struct rb_tree_node; 

template <typename T> struct rb_tree_iterator; 
template <typename T> struct rb_tree_const_iterator; 


// rb tree value type traits 
template <typename T, bool> 
struct rb_tree_value_traits_imp
{
    typedef T key_type; 
    typedef T mapped_type; 
    typedef T value_type; 

    // key 即是 value, value 即是 key 
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

// paritial specialization 
template <typename T> 
struct rb_tree_value_traits_imp<T, true>  
{
    typedef typename std::remove_cv<typename T::first_type>::type  key_type;
    typedef typename T::second_type                                 mapped_type; 
    typedef T                                                       value_type; 

    template <typename Ty>  
    static const value_type& get_value(const Ty& value)
    {
        return value; 
    }
}; 

template <typename T> 
struct rb_tree_value_traits 
{
    static constexpr bool is_map = mystl::is_pair<T>::value; 

    typedef rb_tree_value_traits_imp<T, is_map> value_traits_type;

    typedef typename value_traits_type::key_type        key_type; 
    typedef typename value_traits_type::mapped_type     mapped_type; 
    typedef typename value_traits_type::value_type      value_type; 

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

// rb_tree nodes traits 


template <typename T>   
struct rb_tree_node_traits 
{
    typedef rb_tree_color_type                  color_type; 

    typedef rb_tree_value_traits<T>             value_traits; 
    typedef typename value_traits::key_type     key_type; 
    typedef typename value_traits::mapped_type  mapped_type; 
    typedef typename value_traits::value_type   value_type; 

    typedef rb_tree_node_base<T>*               base_ptr; 
    typedef rb_tree_node<T>*                    node_ptr;
}; 

// rb_tree node base 
template <typename T> 
struct rb_tree_node_base 
{
    typedef rb_tree_color_type              color_type; 
    typedef rb_tree_node_base<T>*           base_ptr; 
    typedef rb_tree_node<T>*                node_ptr; 

    base_ptr    parent; 
    base_ptr    left; 
    base_ptr    right; 
    color_type  color; 

    base_ptr get_base_ptr()
    {
        return &*(this); 
    }

    node_ptr get_node_ptr()
    {
        return reinterpret_cast<node_ptr> (&*this); 
    }

    node_ptr& get_node_ref()
    {
        return reinterpret_cast<node_ptr&>(*this); 
    }
}; 

template <typename T>  
struct rb_tree_node: public rb_tree_node_base<T> 
{
    typedef rb_tree_node_base<T>*       base_ptr; 
    typedef rb_tree_node<T>*            node_ptr; 

    T value; 

    base_ptr get_base_ptr()
    {
        return static_cast<base_ptr> (&*this); 
    }

    node_ptr get_node_ptr() 
    {
        return &*this; 
    }
}; 

// rb_tree traits 
template <typename T> 
struct rb_tree_traits 
{
    typedef rb_tree_value_traits<T>                 value_traits; 
    typedef typename value_traits::key_type         key_type; 
    typedef typename value_traits::mapped_type      mapped_type; 
    typedef typename value_traits::value_type       value_type; 

    typedef value_type*                             pointer; 
    typedef value_type&                             reference; 
    typedef const value_type*                       const_pointer; 
    typedef const value_type&                       const_reference; 

    typedef rb_tree_node_base<T>                    base_type; 
    typedef rb_tree_node<T>                         node_type; 

    typedef base_type*                              base_ptr; 
    typedef node_type*                              node_ptr; 
}; 


// rb_tree  iterator 
template <typename T> 
struct rb_tree_iterator_base: public mystl::iterator<mystl::bidirectional_iterator_tag, T>
{
    typedef typename rb_tree_traits<T>::base_ptr    base_ptr; 

    base_ptr    node; 
    
    rb_tree_iterator_base(): node(nullptr) {} 

    // 使迭代器前进 -> 找到直接后继节点 succ 
    void inc() 
    {
        if(node->right != nullptr)
        {
            node = rb_tree_min(node->right); 
        }
        else 
        {
            // no right child 
            auto p = node->parent; 
            while(p->right == node)
            {
                node = p; 
                p = p->parent; 
            }
            if(node->right !=p ) // 节点没有右孩子
            {
                node = p; 
            }
        }
    }

    void dec() 
    {
        if(node->parent->parent == node && rb_tree_is_red(node))
        {
            // node 为 header 
            node = node ->right; 
        }
        else if(node->left != nullptr)
        {
            node = rb_tree_max(node->left); 
        }
        else
        {
            // not header node, no left child neither 
            auto p = node->parent; 
            while(node == p->left)
            {
                node = p; 
                p = p->parent; 
            }
            node = p; 
        }
    }

    bool operator== (const rb_tree_iterator_base& rhs) {return node == rhs.node; }
    bool operator!= (const rb_tree_iterator_base& rhs) {return node != rhs.node; }
}; 

template <typename T> 
struct rb_tree_iterator: public rb_tree_iterator_base<T> 
{
    typedef rb_tree_traits<T>                           tree_traits; 

    typedef typename tree_traits::value_type            value_type; 
    typedef typename tree_traits::pointer               pointer; 
    typedef typename tree_traits::reference             reference;
    typedef typename tree_traits::base_ptr              base_ptr; 
    typedef typename tree_traits::node_ptr              node_ptr; 

    typedef rb_tree_iterator<T>                         iterator; 
    typedef rb_tree_const_iterator<T>                   const_iterator; 
    typedef iterator                                    self; 


    using rb_tree_iterator_base<T>::node; 

    // 构造函数
    rb_tree_iterator() {} 
    rb_tree_iterator(base_ptr x) {node = x; } 
    rb_tree_iterator(node_ptr x) {node = x; } 
    rb_tree_iterator(const iterator& rhs) {node = rhs.node; } 
    rb_tree_iterator(const const_iterator& rhs) {node = rhs.node; }  

    // operator overload 
    reference   operator*() const {return node->get_node_ptr()->value; } 
    pointer     operator->() const {return &(operator*()); } 

    self& operator++() 
    {
        this->inc(); 
        return *this; 
    }

    self operator++(int)
    {
        self temp(*this); 
        this->inc(); 
        return temp; 
    }

    self& operator--() 
    {
        this->dec(); 
        return *this; 
    }

    self operator--(int)  
    {
        self temp (*this); 
        this->dec(); 
        return temp; 
    }
}; 


template <typename T> 
struct rb_tree_const_iterator: public rb_tree_iterator_base<T> 
{
    typedef rb_tree_traits<T>                       tree_traits; 
    
    typedef typename tree_traits::value_type        value_type; 
    typedef typename tree_traits::const_pointer     pointer; 
    typedef typename tree_traits::const_reference   reference; 
    typedef typename tree_traits::base_ptr          base_ptr;
    typedef typename tree_traits::node_ptr          node_ptr; 

    typedef rb_tree_iterator<T>                     iterator; 
    typedef rb_tree_const_iterator<T>               const_iterator;
    typedef const_iterator                          self; 

    using rb_tree_iterator_base<T>::node; 

    // ctors 
    rb_tree_const_iterator() {} 
    rb_tree_const_iterator(base_ptr x) {node = x; } 
    rb_tree_const_iterator(node_ptr x) {node = x; } 
    rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; } 
    rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; } 

    // operator overloads 
    reference operator*() const { return node->get_node_ptr()->value; } 
    pointer  operator->() const { return &(operator*()); } 

    self& operator++() 
    {
        this->inc(); 
        return *this; 
    } 

    self operator++(int)
    {
        self temp(*this); 
        this->inc(); 
        return temp; 
    }

    self& operator--()
    {
        this->dec(); 
        return *this; 
    }

    self operator--(int)  
    {
        self temp(*this); 
        this->dec(); 
        return temp; 
    }
}; 

// rb_tree operations 
// go along left branch 
template <typename NodePtr> 
NodePtr rb_tree_min(NodePtr x) noexcept
{
    while(nullptr != x->left) 
    {
        x = x->left; 
    }
    return x; 
}

// go along right branch 
template <typename NodePtr> 
NodePtr rb_tree_max(NodePtr x) noexcept 
{
    while(nullptr != x->right)
    {
        x = x->right; 
    }   
    return x; 
}

template <typename NodePtr> 
bool rb_tree_is_lchild(NodePtr node) noexcept 
{
    return node == node->parent->left; 
}

template <typename NodePtr> 
bool rb_tree_is_red(NodePtr node) noexcept 
{
    return node->color == rb_tree_red; 
}

template <typename NodePtr> 
void rb_tree_set_black(NodePtr& node) noexcept
{
    node->color = rb_tree_black; 
}

template <typename NodePtr> 
void rb_tree_set_red(NodePtr& node) noexcept
{
    node->color = rb_tree_red; 
}

// get succ 
template <typename NodePtr> 
NodePtr rb_tree_next(NodePtr node) noexcept
{
    if(node->right != nullptr)
    {
        return rb_tree_min(node->right); 
    }
    while(!rb_tree_is_lchild(node))
    {
        node = node->parent; 
    }
    return node->parent;
}

template <typename NodePtr> 
void rb_tree_rotate_right(NodePtr x, NodePtr& root ) noexcept
{
    auto y = x->left; 
    x->left = y -> right; 
    if(y->right)
    {
        y->right->parent = x; 
    }
    y->parent = x->parent; 

    if(x == root)
    {
        root = y; 
    }
    else if(rb_tree_is_lchild(x))
    {
        x->parent->left = y; 
    }
    else 
    {
        x->parent->right =y; 
    }

    y->right = x; 
    x->parent = y;
}


















}




#endif // ! RBTREE_H_