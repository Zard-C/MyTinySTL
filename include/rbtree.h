#ifndef  RBTREE_H_
#define  RBTREE_H_

// 模版类:rbtree 
#include <initializer_list> 
#include <cassert> 
#include <functional> 
#include "functional.h"
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
struct rb_tree_value_traits 
{
    static constexpr bool is_map = mystl::is_pair<T>::value; 

    typedef rb_tree_value_traits_imp<T, is_map>         value_traits_type;

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
        return &*this; 
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

/*---------------------------------------*\
|       p                         p       |
|      / \                       / \      |
|     x   d    rotate left      y   d     |
|    / \       ===========>    / \        |
|   a   y                     x   c       |
|      / \                   / \          |
|     b   c                 a   b         |
\*---------------------------------------*/

// rotate_left 
template <typename NodePtr> 
void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept 
{
    auto y = x->right; 
    x->right = y->left; 
    if(y->left != nullptr)
    {
        y->left->parent = x; 
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
        x->parent->right = y; 
    }

    y->left = x; 
    x->parent = y; 
}

/*----------------------------------------*\
|     p                         p          |
|    / \                       / \         |
|   d   x      rotate right   d   y        |
|      / \     ===========>      / \       |
|     y   a                     b   x      |
|    / \                           / \     |
|   b   c                         c   a    |
\*----------------------------------------*/
// rotate right 
template <typename NodePtr> 
void rb_tree_rotate_right(NodePtr x, NodePtr& root) noexcept
{
    auto y = x->left; 
    x->left = y->right; 
    if(y->right != nullptr)
    {
        y->right->parent = x; 
    }
    y->parent = x->parent; 

    if(x == root)
    {
        root = y; 
    }

    else if (rb_tree_is_lchild(x))
    {
        x->parent->left = y; 
    }
    else  
    {
        x->parent->right = y; 
    }

    y->right = x; 
    x->parent = y; 
}

// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
//
// case 1: 新增节点位于根节点，令新增节点为黑
// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
//         然后令祖父节点为当前节点，继续处理
// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
//         让父节点成为当前节点，再以当前节点为支点左（右）旋
// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋
template <typename NodePtr> 
void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept 
{
    rb_tree_set_red(x);     // 新插入节点标红
    // 双红修正 
    while(x != root && rb_tree_is_red(x->parent))
    {
        if(rb_tree_is_lchild(x->parent))
        {
            auto uncle = x->parent->parent->right; 
            if(uncle != nullptr && rb_tree_is_red(uncle))
            {
                // case 3 
                rb_tree_set_black(x->parent); 
                rb_tree_set_black(uncle);
                x = x->parent->parent; 
                rb_tree_set_red(x); 
            }
            else  
            {
                // uncle == nullptr or uncle is black 
                if(!rb_tree_is_lchild(x))
                {
                    // case 4 
                    x = x->parent; 
                    rb_tree_rotate_left(x, root); 
                }
                // case 5 
                rb_tree_set_black(x->parent); 
                rb_tree_set_red(x->parent->parent); 
                rb_tree_rotate_right(x->parent->parent, root);                 
                break; 
            }
        }

        else  // parent is a lchild 
        {
            auto uncle = x->parent->parent->left; 
            if(uncle != nullptr && rb_tree_is_red(uncle))
            {
                // case 3 
                rb_tree_set_black(x->parent); 
                rb_tree_set_black(uncle); 
                x = x->parent->parent; 
                rb_tree_set_red(x); 
            }
            else  
            {   
                //uncle is nullptr or uncle is black 
                if(rb_tree_is_lchild(x))
                {
                    //case 4 
                    x = x->parent;
                    rb_tree_rotate_right(x, root); 
                }
                // convert to case 5 
                rb_tree_set_black(x->parent); 
                rb_tree_set_red(x->parent->parent); 
                rb_tree_rotate_left(x->parent->parent, root); 
                break; 
            }
        }
    }
    rb_tree_set_black(root);    // root is always black 
}



// remove node and rebalance rbtree 
template <typename NodePtr> 
NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root,  NodePtr& leftmost, NodePtr& rightmost)
{
    auto y = (z->left == nullptr || z->right == nullptr)? z: rb_tree_next(z); 

    auto x = y->left != nullptr? y->left : y->right; 

    NodePtr xp = nullptr; 

    if(y != z)
    {
        z->left->parent = y; 
        y->left = z->left; 

        if(y != z->right)
        {
            xp = y->parent; 
            if(x != nullptr)
            {
                x->parent = y->parent; 
            }

            y->parent->left = x; 
            y->right = z->right; 
            z->right->parent = y; 
        }
        else  
        {
            xp = y; 
        }

        // connect y and z's parent 
        if(root == z) 
        {
            root = y; 
        }
        else if (rb_tree_is_lchild(z))
        {
            z->parent->left = y; 
        }
        else   
        {
            z->parent->right = y;  
        }
        y->parent = z->parent;  
        swap(y->color, z->color); 
        y = z; 
    }
    else  
    {
        xp = y->parent; 
        if(x!= nullptr) 
        {
            x->parent = y->parent; 
        }

        // connect x and z's parent node 
        if(root == z)
        {
            root = x; 
        }
        else if(rb_tree_is_lchild(z))
        {
            z->parent->left = x; 
        }
        else  
        {
            z->parent->right = x; 
        }

        if(leftmost == z) 
        {
            leftmost = x == nullptr ? xp: rb_tree_min(x); 
        }
        if(rightmost == z) 
        {
            rightmost = x == nullptr? xp: rb_tree_max(x); 
        } 
    }

    if(!rb_tree_is_red(y))
    {   
        while(x != root && (x == nullptr || !rb_tree_is_red(x)))
        {
            if(x == xp->left) 
            {
                auto brother = xp->right; 
                if(rb_tree_is_red(brother))
                {
                    // case 1 
                    rb_tree_set_black(brother);
                    rb_tree_set_red(xp); 
                    rb_tree_rotate_left(xp, root); 
                    brother = xp->right; 
                }

                if((brother->left == nullptr || !rb_tree_is_red(brother->left)) && 
                   (brother->right == nullptr || !rb_tree_is_red(brother->right))) 
                {               
                    // case2 
                    rb_tree_set_red(brother);
                    x = xp; 
                    xp = xp->parent; 
                }
                else  
                {
                    if(brother->right == nullptr || !rb_tree_is_red(brother->right))
                    {
                        // case 3 
                        if(brother ->left != nullptr) 
                        {
                            rb_tree_set_black(brother->left); 
                        }
                        rb_tree_set_red(brother); 
                        rb_tree_rotate_right(brother, root); 
                        brother = xp->right; 
                    }

                    // case 4 
                    brother->color = xp->color; 
                    rb_tree_set_black(xp); 

                    if(brother->right != nullptr) 
                    {
                        rb_tree_set_black(brother->right); 
                    }
                    rb_tree_rotate_left(xp, root); 
                    break; 
                }
            }

            else  // x is right child node 
            {
                auto brother = xp->left; 
                if(rb_tree_is_red(brother))
                {
                    // case 1
                    rb_tree_set_black(brother); 
                    rb_tree_set_red(xp); 
                    rb_tree_rotate_right(xp, root); 
                    brother = xp->left; 
                }

                if((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&  
                   (brother->right == nullptr || !rb_tree_is_red(brother->right)))  
                {
                    // case 2 
                    rb_tree_set_red(brother); 
                    x = xp; 
                    xp = xp->parent; 
                }
                else   
                {
                    if(brother->left == nullptr || !rb_tree_is_red(brother->left))
                    {
                        // case3  
                        if(brother->right != nullptr)
                        {
                            rb_tree_set_black(brother->right); 
                        }
                        rb_tree_set_red(brother); 
                        rb_tree_rotate_left(brother, root); 
                        brother = xp->left; 
                    }

                    // case 4 
                    brother->color = xp->color; 
                    rb_tree_set_black(xp); 
                    if (brother->left != nullptr)
                    {
                        rb_tree_set_black(brother->left); 
                    }
                    rb_tree_rotate_right(xp, root);     
                    break; 
                }
            }
        }
        if(x != nullptr)
        {
            rb_tree_set_black(x); 
        }
    }
    return y; 
}


// rb_tree 
template <typename T, typename Compar>  
class rb_tree  
{
public:
    typedef rb_tree_traits<T>                           tree_traits; 
    typedef rb_tree_value_traits<T>                     value_traits; 

    typedef typename tree_traits::base_type             base_type; 
    typedef typename tree_traits::base_ptr              base_ptr; 
    typedef typename tree_traits::node_type             node_type; 
    typedef typename tree_traits::node_ptr              node_ptr; 
    typedef typename tree_traits::key_type              key_type; 
    typedef typename tree_traits::mapped_type           mapped_type; 
    typedef typename tree_traits::value_type            value_type; 
    typedef Compar                                      key_compare; 

    typedef mystl::allocator<T>                         allocator_type; 
    typedef mystl::allocator<T>                         data_allocator; 
    typedef mystl::allocator<base_type>                 base_allocator; 
    typedef mystl::allocator<node_type>                 node_allocator; 

    typedef typename allocator_type::pointer            pointer; 
    typedef typename allocator_type::const_pointer      const_pointer; 
    typedef typename allocator_type::reference          reference; 
    typedef typename allocator_type::const_reference    const_reference; 
    typedef typename allocator_type::size_type          size_type; 
    typedef typename allocator_type::difference_type    difference_type; 

    typedef rb_tree_iterator<T>                         iterator; 
    typedef rb_tree_const_iterator<T>                   const_iterator; 
    typedef mystl::reverse_iterator<iterator>           reverse_iterator; 
    typedef mystl::reverse_iterator<const_iterator>     const_reverse_iterator; 

    allocator_type get_allocator() const {return data_allocator();  } 
    key_compare     key_comp()     const {return _key_comp; }

private:  
    
    base_ptr        _header;            // 与root 互为父节点 
    size_type       _node_count;        // 节点规模
    key_compare     _key_comp;          // value 比较规则 

private:   
    // auxiliary method  
    base_ptr& root() const { return _header->parent; }
    base_ptr& leftmost() const { return _header->left; } 
    base_ptr& rightmost() const { return _header->right; } 

public:  
    // ctors 
    rb_tree() {rb_tree_init();  } 
    rb_tree(const rb_tree& rhs); 
    rb_tree(rb_tree&& rhs) noexcept; 

    rb_tree& operator= (const rb_tree& rhs);
    rb_tree& operator= (rb_tree&& rhs); 

    ~rb_tree() 
    { 
        clear(); 
        base_allocator::deallocate(_header); // remember to deallocated _header 
    }

public:  
    // iterator options
    iterator                    begin()             noexcept 
    { return leftmost(); } 
    const_iterator              begin()       const noexcept 
    { return leftmost(); }
    iterator                    end()               noexcept 
    { return _header; } 
    const_iterator              end()         const noexcept 
    { return _header; } 

    reverse_iterator            rbegin()            noexcept 
    { return reverse_iterator(end()); } 
    const_reverse_iterator      rbegin()      const noexcept 
    { return const_reverse_iterator(end()); } 
    reverse_iterator            rend()              noexcept
    { return reverse_iterator(begin()); }
    const_reverse_iterator      rend()        const noexcept 
    { return const_reverse_iterator(begin()); } 

    const_iterator              cbegin()      const noexcept
    { return const_iterator(begin()); }
    const_iterator              cend()        const noexcept 
    { return const_iterator(end()); } 
    const_reverse_iterator      crbegin()     const noexcept 
    { return rbegin(); } 
    const_reverse_iterator      crend()       const noexcept 
    { return rend(); } 

    // size 
    bool empty()            const noexcept { return _node_count == 0; } 
    size_type size()        const noexcept { return _node_count; } 
    size_type max_size()    const noexcept { return static_cast<size_type>(-1); } 

    // insert options 
    // emplace 
    template <typename... Args>   
    iterator emplace_multi(Args&&... args); 

    template <typename... Args>  
    mystl::pair<iterator, bool> emplace_unique(Args&& ... args); 

    template <typename... Args> 
    iterator emplace_multi_use_hint(iterator hint, Args&&... args); 

    template <typename... Args>  
    iterator emplace_unique_use_hint(iterator hint, Args&&... args); 

    // insert 
    iterator insert_multi(const value_type& value); 
    iterator insert_multi(value_type&& value)
    {
        return emplace_multi(mystl::move(value)); 
    }

    iterator insert_multi(iterator hint, const value_type& value)
    {
        return emplace_multi_use_hint(hint, value); 
    }
    iterator insert_multi(iterator hint, value_type&& value)
    {
        return emplace_multi_use_hint(hint, mystl::move(value)); 
    }

    template<typename InputIter>  
    void insert_multi(InputIter first, InputIter last) 
    {
        size_type n = mystl::distance(first, last); // O(n) 
        THROW_LENGTH_ERROR_IF(_node_count > max_size() - n, 
            "rb_tree<T, Comp>'s size too big"); 
        for(int i = 0; i < n; ++i, ++first)
        {
            insert_multi(end(), *first); 
        }
    }

    mystl::pair<iterator, bool> insert_unique(const value_type& value); 
    mystl::pair<iterator, bool> insert_unique(value_type&& value)
    {
        return emplace_unique(mystl::move(value)); 
    }

    iterator insert_unique(iterator hint, const value_type& value)
    {
        return emplace_unique_use_hint(hint, value); 
    }

    iterator insert_unique(iterator hint, value_type&& value)
    {
        return emplace_unique_use_hint(hint, mystl::move(value)); 
    }

    template <typename InputIter>  
    void insert_unique(InputIter first, InputIter last)
    {
        size_type n = mystl::distance (first, last); 
        THROW_LENGTH_ERROR_IF(_node_count > max_size() - n, 
                        "rb_tree<T, Comp>'s size too big"); 
        for(int i = 0; i < n; ++i, ++first)
        {
            insert_unique(end(), *first); 
        }
    }

    // erase  
    iterator        erase(iterator hint);

    size_type       erase_multi(const key_type& key); 
    size_type       erase_unique(const key_type& key); 

    void erase(iterator first, iterator last); 

    void clear();     

    // other opeartions 
    iterator            find (const key_type& key); 
    const_iterator      find (const key_type& key) const; 

    size_type   count_multi(const key_type& key) const
    {
        auto p = equal_range_multi(key); 
        return static_cast<size_type> (mystl::distance(p.first, p.second));
    }

    size_type   count_unique(const key_type& key) const 
    {
        return find(key) != end() ? 1: 0; 
    }

    iterator            lower_bound(const key_type& key); 
    const_iterator      lower_bound(const key_type& key ) const; 


    iterator            upper_bound(const key_type& key); 
    const_iterator            upper_bound(const key_type& key) const; 

    mystl::pair<iterator, iterator>  
    equal_range_multi(const key_type& key)
    {
        return mystl::pair<iterator, iterator> (lower_bound(key), upper_bound(key)); 
    }

    mystl::pair<const_iterator, const_iterator> 
    equal_range_multi(const key_type& key) const 
    {
        return mystl::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key)); 
    }

    mystl::pair<iterator, iterator> 
    equal_range_unique(const key_type& key)
    {
        iterator it = find(key);
        iterator next = it; 
        return it == end()? mystl::pair<iterator, iterator>(it, it): mystl::pair<iterator, iterator>(it, ++next);
        //return it == end()? mystl::make_pair(it, it): mystl::make_pair(it, ++next); 
    }

    mystl::pair<const_iterator, const_iterator> 
    equal_range_unique(const key_type& key) const 
    {
        const_iterator it = find(key); 
        auto next = it;
        return it == end()? mystl::make_pair(it, it) : mystl::make_pair(it, ++next); 
    }

    void swap(rb_tree& rhs) noexcept; 

private:  

    // node related 
    template <typename... Args>  
    node_ptr    create_node(Args&& ... args); 
    node_ptr    clone_node(base_ptr x); 
    void        destroy_node(node_ptr p); 

    // init / reset 
    void    rb_tree_init(); 
    void    reset();    

    // get insert postition 
    mystl::pair<base_ptr, bool>  
    get_insert_multi_pos(const key_type& key); 

    mystl::pair<mystl::pair<base_ptr, bool>, bool> 
    get_insert_unique_pos(const key_type& key); 

    // insert value / insert node 
    iterator insert_value_at(base_ptr x, const value_type& value, bool add_to_left); 
    iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left); 

    // insert use hint 
    iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node); 
    iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node); 

    // copy tree / erase tree 
    base_ptr    copy_from(base_ptr x, base_ptr p); 
    void        erase_since(base_ptr x); 
}; 

// implementation 
// ctors 
template <typename T, typename Compar>  
rb_tree<T, Compar> ::  rb_tree(const rb_tree& rhs) 
{
    rb_tree_init(); 
    if(rhs._node_count != 0)
    {
        root() = copy_from(rhs.root(), _header); 
        leftmost() = rb_tree_min(root()); 
        rightmost()  = rb_tree_max(root()); 
    }

    _node_count = rhs._node_count; 
    _key_comp = rhs._key_comp;
}

// move ctor 
template <typename T, typename Compar>  
rb_tree<T, Compar> :: rb_tree(rb_tree&& rhs) noexcept: 
    _header(mystl::move(rhs.header)), 
    _node_count(rhs._node_count), 
    _key_comp(rhs._key_comp)
{
    rhs.reset(); 
}

// copy assign 
template <typename T, typename Compar>  
rb_tree<T, Compar>& rb_tree<T, Compar>::   
operator= (const rb_tree& rhs)
{
    if(this != &rhs)
    {
        clear(); 

        if(rhs._node_count  != 0) 
        {
            root() = copy_from(rhs.root(), _header); 
            leftmost() = rb_tree_min(root()); 
            rightmost() = rb_tree_max(root()); 
        }

        _node_count = rhs._node_count; 
        _key_comp = rhs._key_comp; 
    }
    return *this; 
}

// move assign 
template <typename T, typename Compar>  
rb_tree<T, Compar>& rb_tree<T, Compar>:: operator=(rb_tree&& rhs)
{
    clear(); 
    base_allocator::deallocate(_header);    // don't forget free _header 
    _header = mystl::move(rhs._header); 
    _node_count = rhs._node_count; 
    _key_comp = rhs._key_comp; 
    rhs.reset(); 
    return *this; 
}

// inplace insert element with multi elems 
template <typename T, typename Compar>  
template <typename...Args>  
typename rb_tree<T,Compar>::iterator 
rb_tree<T, Compar>:: emplace_multi(Args&& ...args)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() - 1,     
            "rb_tree<T, Comp>'s size too big!"); 

    node_ptr np = create_node(mystl::forward<Args>(args)...); 
    auto res = get_insert_multi_pos(value_traits::get_key(np->value)); 
    return insert_node_at(res.first, np, res.second); 
}

// inplace insert element unique elems 
template <typename T, typename Compar>  
template <typename ...Args>  
mystl::pair<typename rb_tree<T, Compar>::iterator, bool>  
rb_tree<T, Compar>:: emplace_unique(Args&& ...args)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() -1, 
                "rb_tree<T, Comp>'s size too big"); 
    node_ptr np = create_node(mystl::forward<Args>(args)...); 
    auto res = get_insert_unique_pos(value_traits::get_key(np->value)); 

    if(res.second)
    {
        // insert succeed
        return mystl::make_pair(insert_node_at(res.first.first, np, res.first.second), true); 
    }
    
    destroy_node(np); 
    return mystl::make_pair(iterator(res.first.first), false);
}

// inplace insert allow multi keys, use hint is more effictive when hint is close to
// insert position 
template <typename T, typename Compar>  
template <typename ... Args>  
typename rb_tree<T, Compar>::iterator 
rb_tree<T, Compar>:: emplace_multi_use_hint(iterator hint, Args&& ...args)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() - 1,  
                            "rb_tree<T, Comp>'s size too big"); 
    node_ptr np = create_node(mystl::forward<Args>(args)...); 
    if(_node_count == 0)
    {
        return insert_node_at(_header, np, true); 
    }
    
    key_type key = value_traits::get_key(np->value); 
    if(hint == begin())
    {
        if(_key_comp(key, value_traits::get_key(*hint)))
        {
            return insert_node_at(hint.node, np, true);
        }
        else   
        {
            auto pos = get_insert_multi_pos(key); 
            return insert_node_at(pos.first, np, pos.second); 
        }
    }
    else if( hint == end())
    {
        if(!_key_comp(key, value_traits::get_key(rightmost()->get_node_ptr()->value)))
        {
            return insert_node_at(rightmost(), np, false); 
        }
        else  
        {
            auto pos = get_insert_multi_pos(key);
            return insert_node_at(pos.first, np, pos.second); 
        }
    }
    return insert_multi_use_hint(hint, key, np);
}

// inplace insert unique key, it's move effective when hit is close to insert position
template <typename T, typename Compar>  
template <typename... Args>  
typename rb_tree<T, Compar>::iterator  
rb_tree<T, Compar>:: emplace_unique_use_hint(iterator hint, Args&&...args)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() - 1, 
                        "rb_tree<T, Comp>'s size too big"); 
    node_ptr np = create_node(mystl::forward<Args>(args)...); 
    if(_node_count == 0)
    {
        return insert_node_at(_header, np, true); 
    }  
    key_type key = value_traits::get_key(np->value); 
    if(hint == begin())
    {
        if(_key_comp(key, value_traits::get_key(*hint)))
        {
            return insert_node_at(hint.node, np, true); 
        }
        else  
        {
            auto pos = get_insert_unique_pos(key); 
            if(!pos.second)
            {
                destroy_node(np); 
                return pos.first.first; 
            }
            return insert_node_at(pos.first.first, np, pos.first.second); 

        }
    }
    else if (hint == end())
    {
        if(_key_comp(value_traits::get_key(rightmost()->get_node_ptr()->value), key))
        {
            return insert_node_at(rightmost(), np, false); 
        }
        else  
        {
            auto pos = get_insert_unique_pos(key); 
            if(!pos.second)
            {
                destroy_node(np); 
                return pos.first.first; 
            }
            return insert_node_at(pos.first.first, np, pos.first.second); 
        }
    }
    return insert_unique_use_hint(hint, key, np); 
}

// insert allows multi keys 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::iterator 
rb_tree<T, Compar>:: insert_multi(const value_type& value)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() - 1, 
                    "rb_tree<T, Comp>'s size too big"); 
    auto res = get_insert_multi_pos(value_traits::get_key(value));
    return insert_value_at(res.first, value, res.second); 
}

// insert unique keys 
template <typename T, typename Compar>  
mystl::pair<typename rb_tree<T, Compar>::iterator, bool>  
rb_tree<T, Compar>::  
insert_unique(const value_type& value)
{
    THROW_LENGTH_ERROR_IF(_node_count > max_size() - 1, 
                    "rb_tree<T, Comp>'s size too big"); 
    auto res = get_insert_unique_pos(value_traits::get_key(value)); 
    if(res.second)
    {
        return mystl::
            make_pair(insert_value_at(res.first.first, value, res.first.second), true); 
    }
    return mystl::make_pair(res.first.first, false); 
}

// remove node at hint 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::iterator   
rb_tree<T, Compar>::erase(iterator hint)
{
    auto node = hint.node->get_node_ptr(); 
    iterator next(node); 
    ++next; 

    rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost()); 
    destroy_node(node);
    --_node_count; 
    return next;
}

// remove elems which equals to key and return num of elems 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>:: size_type  
rb_tree<T, Compar>:: erase_multi(const key_type& key)
{
    auto p = equal_range_multi(key); 
    size_type n = mystl::distance(p.first, p.second); 
    erase(p.first, p.second); 
    return n; 
}


// remove elem which equals to key and return num of elem 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>:: size_type  
rb_tree<T, Compar>:: erase_unique(const key_type& key)
{
    auto it = find(key); 
    if(it != end()) 
    {
        erase(it); 
        return 1; 
    }
    return 0; 
}

// erase [first, last) 
template <typename T, typename Compar>  
void rb_tree<T, Compar>::  
erase(iterator first, iterator last)
{   
    if(first == begin() && last == end())
    {
        clear(); 
    }
    else   
    {
        while(first != last ) 
        {
            erase(first++);
        }
    }
}

// clear 
template <typename T, typename Compar>  
void rb_tree<T, Compar>:: clear()  
{
    if(_node_count != 0)
    {
        erase_since(root()); 
        leftmost() = _header; 
        root() = nullptr; 
        rightmost() = _header; 
        _node_count = 0; 
    }
}

// find key and return its iterator 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::iterator
rb_tree<T, Compar>::find (const key_type& key)
{
    auto y = _header; 
    auto x = root(); 
    while(x != nullptr)
    {
        if(!_key_comp(value_traits::get_key(x->get_node_ptr()->value), key))
        {
            y = x; x = x->left;
        }   
        else   
        {
            x = x->right;
        }
    }
    iterator ite = iterator(y);
    return (ite == end() || _key_comp(key, value_traits::get_key(*ite))) ? end() : ite; 
}

template <typename T, typename Compar>  
typename rb_tree<T, Compar>::const_iterator 
rb_tree<T, Compar> :: find (const key_type& key) const  
{
    auto y = _header; 
    auto x = root(); 
    while( x!= nullptr) 
    {
        if(!_key_comp(value_traits::get_key(x->get_node_ptr()->value), key))
        {
            y = x; x = x->left; 
        }
        else  
        {
            x = x->right; 
        }
    }
    const_iterator ite = const_iterator(y); 
    return (ite == end() || _key_comp(key, value_traits::get_key(*ite))) ? end() : ite; 
}


// lower_bound 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::iterator 
rb_tree<T, Compar>:: lower_bound(const key_type& key)
{
    auto y = _header; 
    auto x = root(); 

    while(x != nullptr)
    {
        if(!_key_comp(value_traits::get_key(x->get_node_ptr()->value), key))
        {
            y = x; x = x->left; 
        }   
        else  
        {
            x = x->right; 
        }
    }
    return iterator(y); 
}

template <typename T, typename Compar>  
typename rb_tree<T, Compar>::const_iterator  
rb_tree<T, Compar>:: lower_bound(const key_type& key) const  
{
    auto y = _header; 
    auto x = root();
    while(x != nullptr)
    {
        if(!_key_comp(value_traits::get_key(x->get_node_ptr()->value), key))
        {
            y = x; x = x->left; 
        }
        else  
        {
            x = x->right; 
        }
    }
    return const_iterator(y); 
}

// upper_bound 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>:: iterator   
rb_tree<T, Compar>:: upper_bound(const key_type& key) 
{
    auto y = _header; 
    auto x = root(); 

    while(x != nullptr)
    {
        if(_key_comp(key, value_traits::get_key(x->get_node_ptr()->value)))
        {
            y = x; x = x->left; 
        }
        else  
        {
            x = x->right; 
        }
    }
    return iterator(y); 
}

template <typename T, typename Compar> 
typename rb_tree<T, Compar>:: const_iterator  
rb_tree<T, Compar>:: upper_bound(const key_type& key) const 
{
    auto y = _header; 
    auto x = root(); 

    while(x != nullptr)
    {
        if(_key_comp(key, value_traits::get_key(x->get_node_ptr()->value)))
        {
            y = x; x = x->left; 
        }
        else  
        {
            x = x->right; 
        }
    }
    return const_iterator(y);
}

// swap rbtree with another 
template <typename T, typename Compar>  
void rb_tree<T, Compar>:: swap(rb_tree& rhs)  noexcept 
{
    if(this != &rhs)
    {
        mystl::swap(_header, rhs._header); 
        mystl::swap(_node_count, rhs._node_count); 
        mystl::swap(_key_comp, rhs._key_comp); 
    }
}

// auxiliary methods  

// create node  
template <typename T, typename Compar>  
template <typename... Args>  
typename rb_tree<T, Compar>:: node_ptr   
rb_tree<T, Compar>:: create_node (Args&&... args)
{
    auto temp = node_allocator::allocate(1);
    try
    {
        data_allocator::construct(mystl::address_of(temp->value), mystl::forward<Args>(args)...); 
        temp->left = nullptr; 
        temp->right = nullptr; 
        temp->parent = nullptr; 
    }
    catch(...)
    {
        node_allocator::deallocate(temp); 
        throw; 
    }
    return temp;  
}

// copy a node 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::node_ptr 
rb_tree<T, Compar>:: clone_node(base_ptr x)
{
    node_ptr temp = create_node(x->get_node_ptr()->value); 
    temp->color = x->color; 
    temp->left = nullptr; 
    temp->right = nullptr; 
    return temp; 
}

// destroy node 
template <typename T, typename Compar> 
void rb_tree<T, Compar>:: destroy_node (node_ptr p)
{
    data_allocator::destroy(&p->value); 
    node_allocator::deallocate(p); 
} 

// initialize container 
template <typename T, typename Compar> 
void rb_tree<T, Compar>:: rb_tree_init ()
{
    _header = base_allocator::allocate(1); 
    _header->color = rb_tree_red; 
    root() = nullptr; 
    leftmost() = _header; 
    rightmost() = _header; 
    _node_count = 0;
}

// reset 
template <typename T, typename Compar>  
void rb_tree<T, Compar>:: reset ()
{
    _header = nullptr; 
    _node_count = 0; 
}

// get_insert_multi_pos 
template <typename T, typename Compar>  
mystl::pair<typename rb_tree<T, Compar>::base_ptr, bool>  
rb_tree<T, Compar>::get_insert_multi_pos(const key_type& key)
{
    auto x = root(); 
    auto y = _header; 

    bool add_to_left = true; 
    while(x != nullptr)
    {
        y =x ; 
        add_to_left = _key_comp(key, value_traits::get_key(x->get_node_ptr()->value)); 
        x = add_to_left ? x->left : x ->right; 
    }
    return mystl::make_pair(y, add_to_left); 
}

// get_insert_unique_pos 
template <typename T, typename Compar>  
mystl::pair<mystl::pair<typename rb_tree<T, Compar>::base_ptr, bool>, bool> 
rb_tree<T, Compar>::get_insert_unique_pos(const key_type& key)
{
    auto x = root(); 
    auto y = _header; 
    bool add_to_left = true; 

    
#if 1
    while(x != nullptr) 
    {   
        y = x; 
        add_to_left = _key_comp(key, value_traits::get_key(x->get_node_ptr()->value)); 
        x = add_to_left ? x->left : x->right; 
    }
    iterator ite = iterator(y);

    if(add_to_left)
    {
        if(y == _header || ite == begin())
        {
            return mystl::make_pair(mystl::make_pair(rb_tree<T, Compar>::base_ptr(y), true), true); 
        }
        else  
        {
            --ite; 
        }
    }
    
    if(_key_comp(value_traits::get_key(*ite), key))
    {
        return mystl::make_pair(mystl::make_pair(rb_tree<T, Compar>::base_ptr(y), mystl::move(add_to_left)), true); 
    }

    return mystl::make_pair(mystl::make_pair(rb_tree<T, Compar>::base_ptr(y), mystl::move(add_to_left)), false); 
    #endif 
}

// insert_value_at  
// x is the parent of the insert node 
 template <typename T, typename Compar>  
typename rb_tree<T, Compar>:: iterator 
rb_tree<T, Compar>:: insert_value_at(base_ptr x, const value_type& value, bool add_to_left)
{
    node_ptr node = create_node(value); 
    node->parent = x; 

    auto base_node = node->get_base_ptr(); 
    if(x == _header)
    {
        root() = base_node; 
        leftmost() = base_node; 
        rightmost() = base_node;    
    }
    else if (add_to_left)
    {
        x->left = base_node; 
        if(leftmost() == x) 
        {
            leftmost() = base_node; 
        }
    }   
    else  
    {
        x->right = base_node; 
        if(rightmost() == x)
        {
            rightmost() = base_node; 
        }
    }
    rb_tree_insert_rebalance(base_node, root()); 
    ++_node_count;
    return iterator(node);  
}

// insert new node at x 
template<typename T, typename Compar>  
typename rb_tree<T, Compar>::iterator 
rb_tree<T, Compar> :: insert_node_at(base_ptr x, node_ptr node, bool add_to_left)
{
    node->parent = x; 
    auto base_node = node->get_base_ptr(); 
    if(x == _header) 
    {
        root() = base_node;
        leftmost() = base_node; 
        rightmost() = base_node; 
    }
    else if (add_to_left) 
    {
        x->left = base_node; 
        if(leftmost() == x) 
        {
            leftmost() = base_node; 
        }
    }
    else  
    {
        x->right = base_node; 
        if(rightmost() == x) 
        {
            rightmost() = base_node; 
        }
    }
    rb_tree_insert_rebalance(base_node, root()); 
    ++_node_count;  
    return iterator(node); 
}

// insert mutli elems use hint 
template <typename T, typename Compar> 
typename rb_tree<T, Compar>::iterator   
rb_tree<T, Compar>::
insert_multi_use_hint(iterator hint, key_type key,  node_ptr node)
{
    // hint 
    auto np = hint.node; 
    auto before = hint; 
    --before; 
    auto bnp = before.node; 
    if(!_key_comp(key, value_traits::get_key(*before)) && 
       !_key_comp(value_traits::get_key(*hint), key))
    {
        if(bnp->right == nullptr) 
        {
            return insert_node_at(bnp, node, false); 
        }
        else if (np->left == nullptr)
        {
            return insert_node_at(np, node, true); 
        }
    }
    auto pos = get_insert_multi_pos(key); 
    return insert_node_at(pos.first, node, pos.second); 
}

// insert unique elems use hint 
template <typename T, typename Compar> 
typename rb_tree<T, Compar>::iterator  
rb_tree<T, Compar>:: 
insert_unique_use_hint(iterator hint, key_type key, node_ptr node)
{
    auto np = hint.node; 
    auto before = hint;
    --before; 
    auto bnp = before.node; 
    if(_key_comp(value_traits::get_key(*before), key) && 
        _key_comp(key, value_traits::get_key((*hint)))) 
    {
        // before < node < hint 
        if(bnp->right == nullptr)
        {
            return insert_node_at(bnp, node, false ); 
        }
        else if(np->left == nullptr)
        {
            return insert_node_at(np, node, true); 
        }
    } 

    auto pos = get_insert_unique_pos(key); 
    if(!pos.second)
    {
        destroy_node(node); 
        return pos.first.first; 
    }
    return insert_node_at(pos.first.first, node, pos.first.second); 
}

// copy_from 
// copy a rb_tree recursively start from x, p is the parent of x 
template <typename T, typename Compar>  
typename rb_tree<T, Compar>::base_ptr
rb_tree<T, Compar>::copy_from(base_ptr x, base_ptr p)
{
    auto top = clone_node(x); 
    top->parent = p; 
    try
    {
        if(x->right)
        {
            top->right = copy_from(x->right, top); 
        }
        p = top; 
        x = x->left; 

        while(x != nullptr)
        {
            auto y = clone_node(x); 
            p->left = y; 
            y->parent = p; 
            if(x->right != nullptr)
            {
                y->right = copy_from(x->right, y); 
            }
            p = y; 
            x = x->left; 
        }
    }
    catch(...)
    {
        erase_since(top); 
        throw; 
    }
    return top; 
}


// erase_since 
// delete node x and its subtree
template <typename T, typename Compar>  
void rb_tree<T, Compar>::erase_since(base_ptr x)
{
    while(x != nullptr)
    {
        erase_since(x->left); 
        auto y = x->right; 
        destroy_node(x->get_node_ptr()); 
        x = y; 
    }
}

// relational operator overloads 
template <typename T, typename Compar>  
bool operator== (const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()); 
}

template <typename T, typename Compar>  
bool operator< (const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); 
}

template <typename T, typename Compar>  
bool operator!= (const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return !(rhs == lhs); 
}

template <typename T, typename Compar>  
bool operator >(const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return rhs < lhs; 
}

template <typename T, typename Compar> 
bool operator <=(const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return !(rhs < lhs);  
}

template <typename T, typename Compar>  
bool operator >=(const rb_tree<T, Compar>& lhs, const rb_tree<T, Compar>& rhs)
{
    return !(lhs < rhs);
}

// overloading generic swap 
template <typename T, typename Compar>  
void swap(rb_tree<T, Compar>& lhs, rb_tree<T, Compar>& rhs)
{
    lhs.swap(rhs); 
}

} // end of namespace mystl 




#endif // ! RBTREE_H_