// map.h include map and multimap which are container adapters
#ifndef MAP_H_
#define MAP_H_  

// notes: map<Key, T> / multimap<Key, T> 满足基本异常保证，对以下函数做强制异常安全保证
// *emplace 
// *empalce_hint
// *insert 

#include "rbtree.h"


namespace mystl
{
template<typename Key, typename T, typename Compar = mystl::less<Key>> 
class map  
{
public: 
    typedef Key                             key_type; 
    typedef T                               mapped_type; 
    typedef mystl::pair<const Key, T>       value_type; 
    typedef Compar                          key_compare; 

    // functor for comparing 
    class value_compare:public mystl::binary_function<value_type, value_type, bool> 
    {
        friend class map<Key, T, Compar> ; 
    private:  
        Compar   comp; 
        value_compare(Compar c): comp(c) {}  
    public:  
        bool operator()(const value_type& lhs, const value_type& rhs) const 
        {
            return comp(lhs.first, rhs.first); 
        }
    }; 

private: 
    // base on rbtree
    typedef mystl::rb_tree<value_type, key_compare>         base_type; 
    base_type                                               _tree;

public: 
    // using rb_tree traits 
    typedef typename base_type::node_type                   node_type; 
    typedef typename base_type::pointer                     pointer; 
    typedef typename base_type::const_pointer               const_pointer; 
    typedef typename base_type::reference                   reference; 
    typedef typename base_type::const_reference             const_reference; 
    typedef typename base_type::iterator                    iterator; 
    typedef typename base_type::const_iterator              const_iterator; 
    typedef typename base_type::reverse_iterator            reverse_iterator; 
    typedef typename base_type::const_reverse_iterator      const_reverse_iterator; 
    typedef typename base_type::size_type                   size_type; 
    typedef typename base_type::difference_type             difference_type; 
    typedef typename base_type::allocator_type              allocator_type; 

public:  
    // ctors 
    map() = default; 

    template <typename InputIter>  
    map(InputIter first, InputIter last): _tree()
    {
        _tree.insert_unique(first, last); 
    }

    map(std::initializer_list<value_type> initlist): _tree()
    {
        _tree.insert_unique(initlist.begin(), initlist.end());
    }

    // copy ctor
    map(const map& rhs): _tree(rhs._tree)
    {

    }

    // move ctor 
    map(map&& rhs):_tree(mystl::move(rhs.tree))
    {

    }   

    // copy assign 
    map& operator= (const map& rhs)
    {
        _tree = rhs._tree; 
        return *this; 
    }

    // move assign 
    map& operator= (map&& rhs)
    {
        _tree = mystl::move(rhs._tree); 
        return *this;
    }

    map& operator= (std::initializer_list<value_type> initlist)
    {
        _tree.clear(); 
        _tree.insert_unique(initlist.begin(), initlist.end()); 
        return *this; 
    }

    // 
    key_compare     key_comp()      const { return _tree.key_comp(); } 
    value_compare   value_comp()    const { return value_compare(_tree.key_comp()); } 
    allocator_type  get_allocator() const { return _tree.get_allocator(); } 

    // iterator 
    iterator        begin()         noexcept 
    {
        return _tree.begin(); 
    }
    const_iterator  begin()         const noexcept 
    {
        return _tree.begin(); 
    }
    iterator        end()           noexcept 
    {
        return _tree.end();     
    }
    const_iterator  end()           const noexcept
    {
        return _tree.end(); 
    }

    reverse_iterator        rbegin()        noexcept 
    { return reverse_iterator(end()); } 
    const_reverse_iterator  rbegin()        const noexcept
    { return const_reverse_iterator(end()); } 
    reverse_iterator        rend()          noexcept 
    { return reverse_iterator(begin()); } 
    const_reverse_iterator  rend()          const noexcept 
    { return const_reverse_iterator(begin()); } 

    const_iterator      cbegin()            const noexcept 
    {return begin();  }
    const_iterator      cend()              const noexcept 
    {return end(); } 
    const_reverse_iterator  crbegin()       const noexcept
    {return rbegin(); } 
    const_reverse_iterator crend()          const noexcept 
    {return rend(); }


    // capacity 
    bool    empty()         const noexcept { return _tree.empty(); }
    size_type   size()      const noexcept { return _tree.size(); }
    size_type   max_size()  const noexcept { return _tree.max_size(); }

    // traverse elems 

    mapped_type& at(const key_type& key)
    {
        iterator it = lower_bound(key); 
        THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key), 
            "map<Key, T> no such element exists"); 
        return it->second; 
    }

    mapped_type& operator[](const key_type& key)
    {
        iterator it = lower_bound(key); 
        // 
        if(it == end() || key_comp()(key, it->first))
        {
            it = emplace_hint(it, key, T{}); 
        }
        return it->second; 
    }

    mapped_type& operator[](key_type&& key)
    {
        iterator it = lower_bound(key); 
        if( it == end() || key_comp()(key, it->first))
        {
            it = emplace_hint(it, mystl::move(key), T{}); 
        }
        return it->second; 
    }

    // insert 
    template <typename ...Args>  
    pair<iterator, bool> emplace (Args&& ...args)
    {
        return _tree.emplace_unique(mystl::forward<Args>(args)...); 
    }

    template <typename ...Args>  
    iterator emplace_hint(iterator hint, Args&& ...args)
    {
        return _tree.emplace_unique_use_hint(hint, mystl::forward<Args>(args)...); 
    }

    pair<iterator, bool> insert(const value_type& value)
    {
        return _tree.insert_unique(value); 
    }

    pair<iterator, bool> insert(value_type&& value)
    {
        return _tree.insert_unique(mystl::move(value)); 
    }

    iterator insert(iterator hint, const value_type& value)
    {
        return _tree.insert_unique(hint, value); 
    }

    iterator insert(iterator hint, value_type&& value)
    {
        return _tree.insert_unique(hint, mystl::move(value)); 
    }

    template<typename InputIter>   
    void insert(InputIter first, InputIter last)
    {
        _tree.insert_unique(first, last); 
    }

    void        erase(iterator position)                { _tree.erase(position); } 
    size_type   erase (const key_type& key)             { return _tree.erase_unique(key); } 
    void        erase(iterator first, iterator last)    { _tree.erase(first, last); }  

    void    clear()                                     { _tree.clear(); }

    // map operation 

    iterator        find(const key_type& key)                 { return _tree.find(key); } 
    const_iterator  find(const key_type& key)           const { return _tree.find(key); } 

    size_type       count(const key_type& key)          const { return _tree.count_multi(key); } 

    iterator        lower_bound(const key_type& key)          { return _tree.lower_bound(key); } 
    const_iterator  lower_bound(const key_type& key)    const { return _tree.lower_bound(key); } 

    iterator        upper_bound(const key_type& key)          { return _tree.upper_bound(key); } 
    const_iterator  upper_bound(const key_type& key)    const { return _tree.upper_bound(key); } 


    pair<iterator, iterator> equal_range(const key_type& key)
    { return _tree.equal_range_unique(key); } 

    pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    { return _tree.equal_range_unique(key); } 

    void swap(map& rhs) noexcept
    { _tree.swap(rhs._tree); } 

public:   
    friend bool operator== (const map& lhs, const map& rhs) { return lhs._tree == rhs._tree; } 
    friend bool operator < (const map& lhs, const map& rhs) { return lhs._tree < rhs._tree; } 
}; 

// overload operator 
template <typename Key, typename T, typename Compar>  
bool operator== (const map<Key, T, Compar>& lhs, const map<Key, T, Compar>& rhs)
{
    return lhs == rhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator < (const map<Key, T, Compar>& lhs, const map<Key, T, Compar>& rhs)
{
    return lhs < rhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator!= (const map<Key, T, Compar>& lhs, const map<Key, T, Compar>& rhs)
{
    return !(rhs == lhs); 
}

template <typename Key, typename T, typename Compar>  
bool operator> (const map<Key, T, Compar>& lhs, const map<Key, T, Compar>& rhs)
{
    return rhs < lhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator <= (const map<Key, T, Compar>& lhs, const map<Key, T, Compar>& rhs)
{
    return !(rhs < lhs); 
}

// overload generic swap 
template <typename Key, typename T, typename Compar> 
void swap(map<Key, T, Compar>& lhs, map<Key, T, Compar>& rhs) noexcept
{
    lhs.swap(rhs);
}

// multimap 
template <typename Key, typename T, typename Compar = mystl::less<Key>> 
class multimap  
{
public: 
    // typedefs 
    typedef Key                         key_type; 
    typedef T                           mapped_type; 
    typedef mystl::pair<const Key, T>   value_type; 
    typedef Compar                      key_compare; 

    // functor for comparision 
    struct value_compare: public binary_function<value_type, value_type, bool> 
    {
        friend class multimap<Key, T, Compar>; 
    private:   
        Compar      comp; 
        value_compare(Compar c): comp(c) {} 
    public:  
        bool operator()(const value_type& lhs, const value_type& rhs) const  
        {
            return comp(lhs.first, rhs.first); 
        }
    }; 

private:  
    typedef mystl::rb_tree<value_type, key_compare>         base_type; 
    base_type   _tree; 

public:  
    // using rb_tree traits 
    typedef typename base_type::node_type                   node_type; 
    typedef typename base_type::pointer                     pointer; 
    typedef typename base_type::const_pointer               const_pointer; 
    typedef typename base_type::reference                   reference; 
    typedef typename base_type::const_reference             const_reference; 
    typedef typename base_type::iterator                    iterator; 
    typedef typename base_type::const_iterator              const_iterator; 
    typedef typename base_type::reverse_iterator             reverse_iterator; 
    typedef typename base_type::const_reverse_iterator      const_reverse_iterator; 
    typedef typename base_type::size_type                   size_type; 
    typedef typename base_type::difference_type             difference_type; 
    typedef typename base_type::allocator_type              allocator_type; 

public:   
    // ctors 
    multimap() = default; 

    template <typename InputIter>  
    multimap(InputIter first, InputIter last): _tree() 
    {
        _tree.insert_multi(first, last); 
    }

    multimap(std::initializer_list<value_type> ilist): _tree() 
    {
        _tree.insert_multi(ilist.begin(), ilist.end()); 
    } 

    multimap(const multimap& rhs): _tree(rhs._tree)
    {

    } 

    multimap(multimap&& rhs) noexcept :_tree(mystl::move(rhs._tree))
    {

    }

    multimap& operator= (const multimap& rhs) 
    {
        _tree = rhs._tree; 
        return *this; 
    }

    multimap& operator= (multimap&& rhs)
    {
        _tree = mystl::move(rhs._tree); 
        return *this; 
    }

    multimap& operator=(std::initializer_list<value_type> ilist)
    {
        _tree.clear(); 
        _tree.insert_multi(ilist.begin(), ilist.end()); 
        return *this; 
    }

    // 
    key_compare                 key_comp()      const {return _tree.key_comp(); } 
    value_compare               value_comp()    const {return value_compare(_tree.key_comp()); } 
    allocator_type              get_allocator() const {return _tree.get_allocator(); }

    // iterator 
    iterator                begin()             noexcept 
    { return _tree.begin(); } 
    const_iterator          begin()             const noexcept  
    { return _tree.begin(); }          
    iterator                end()               noexcept 
    { return _tree.end(); } 
    const_iterator          end()               const noexcept 
    { return _tree.end(); } 

    reverse_iterator        rbegin()            noexcept 
    { return reverse_iterator(end()); } 
    const_reverse_iterator  rbegin()            const noexcept  
    { return const_reverse_iterator(end()); } 
    reverse_iterator        rend()              noexcept 
    { return reverse_iterator(begin()); }        
    const_reverse_iterator  rend()              const noexcept  
    {return const_reverse_iterator(begin()); }      

    const_iterator          cbegin()            const noexcept  
    { return begin(); } 
    const_iterator          cend()              const noexcept  
    { return end(); } 
    const_reverse_iterator  crbegin()           const noexcept  
    { return rbegin(); } 
    const_reverse_iterator  crend()             const noexcept 
    { return rend(); } 

    // capacity  
    bool               empty() const noexcept { return _tree.empty(); } 
    size_type          size()  const noexcept { return _tree.size(); } 
    size_type          max_size() const noexcept { return _tree.max_size(); } 

    // insert & erase 

    template <typename...Args>  
    iterator emplace(Args&& ...args) 
    {
        return _tree.emplace_multi(mystl::forward<Args>(args)...); 
    }

    template <typename...Args>  
    iterator emplace_hint(iterator hint, Args&&... args) 
    {
        return _tree.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...); 
    }

    iterator insert(const value_type& value)
    {
        return _tree.insert_multi(value); 
    }

    iterator insert(value_type&& value) 
    {
        return _tree.insert_multi(mystl::move(value)); 
    }

    iterator insert(iterator hint, const value_type& value)
    {
        return _tree.insert_multi(hint, value); 
    }

    iterator insert(iterator hint, value_type&& value)
    {
        return _tree.insert_multi(hint, mystl::move(value)); 
    }

    template <typename InputIter>  
    void insert(InputIter first, InputIter last)
    {
        _tree.insert_multi(first, last); 
    }

    void            erase(iterator position)            
    { _tree.erase(position); } 
    size_type       erase(const key_type& key)          
    { return _tree.erase_multi(key); }
    void            erase(iterator first, iterator last)  
    { _tree.erase(first, last);  } 

    void clear() {_tree.clear(); }

    // multimap -> search 

    iterator        find(const key_type& key)    
    { 
        return _tree.find(key); 
    } 
    const_iterator  find(const key_type& key)  const 
    {
        return _tree.find(key); 
    }  

    size_type       count(const key_type& key)  const 
    {
        return _tree.count_multi(key); 
    } 

    iterator        lower_bound(const key_type& key) 
    {
        return _tree.lower_bound(key); 
    }

    const_iterator  lower_bound(const key_type& key)  const 
    {
        return _tree.lower_bound(key); 
    }

    iterator        upper_bound(const key_type& key)    
    {
        return _tree.upper_bound(key); 
    }

    const_iterator  upper_bound(const key_type& key) const  
    {
        return _tree.upper_bound(key); 
    }

    pair<iterator, iterator> equal_range(const key_type& key)
    {
        return _tree.equal_range_multi(key); 
    }    

    pair<const_iterator, const_iterator> equal_range(const key_type& key) const 
    {
        return _tree.equal_range_multi(key); 
    }

    void swap(multimap& rhs) noexcept
    {
        _tree.swap(rhs._tree); 
    }

public:  
    friend bool operator==(const multimap& lhs, const multimap& rhs) 
    {
        return lhs._tree == rhs._tree; 
    }

    friend bool operator<(const multimap& lhs, const multimap& rhs)
    {
        return lhs._tree < rhs._tree; 
    }
 }; 

// relational operator oveloading 
template <typename Key, typename T, typename Compar>  
bool operator == (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return lhs == rhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator< (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return lhs < rhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator!= (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return !(lhs == rhs); 
}

template <typename Key, typename T, typename Compar>   
bool operator> (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return rhs < lhs; 
}

template <typename Key, typename T, typename Compar>  
bool operator<= (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return !(rhs < lhs);
}

template <typename Key, typename T, typename Compar>  
bool operator>= (const multimap<Key, T, Compar>& lhs, const multimap<Key, T, Compar>& rhs)
{
    return !(lhs < rhs); 
}

// overloading generic swap 
template <typename Key, typename T, typename Compar>  
void swap(multimap<Key, T, Compar>& lhs, multimap<Key, T, Compar>& rhs)
{
    lhs.swap(rhs); 
}
}   // end of mystl 
#endif // !MAP_H_