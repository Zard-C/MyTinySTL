// set.h -- contains set and multiset 
#ifndef SET_H_
#define SET_H_  

// notes: 

// 异常保证: 
// mystl::set<Key> / mystl::multiset<Key> 满足基本异常保证，对以下函数做强异常
// 安全保证
// * emplace
// * emplace_hint
// * insert 

#include "rbtree.h"

namespace mystl
{

// template class set 
template <typename Key, typename Compar = mystl::less<Key>>  
class set  
{   
public: 
    typedef Key                     key_type; 
    typedef Key                     value_type; 
    typedef Compar                  key_compare; 
    typedef Compar                  value_compare; 

private: 
    typedef mystl::rb_tree<value_type, key_compare>      base_type; 
    base_type                                            _tree; 

public: 
    typedef typename base_type::node_type               node_type; 
    typedef typename base_type::const_pointer           pointer; 
    typedef typename base_type::const_pointer           const_pointer; 
    typedef typename base_type::const_reference         reference;
    typedef typename base_type::const_reference         const_reference; 
    typedef typename base_type::const_iterator          iterator; 
    typedef typename base_type::const_iterator          const_iterator; 
    typedef typename base_type::const_reverse_iterator  reverse_iterator; 
    typedef typename base_type::const_reverse_iterator  const_reverse_iterator;
    typedef typename base_type::size_type               size_type; 
    typedef typename base_type::difference_type         difference_type; 
    typedef typename base_type::allocator_type          allocator_type; 

public:   
    // ctors 
    set() = default; 

    // range init 
    template <typename InputIter>  
    set(InputIter first, InputIter last): _tree() 
    {
        _tree.insert_unique(first, last); 
    }
    
    // initializer_list 
    set(std::initializer_list<value_type> ilist): _tree()
    {
        _tree.insert_unique(ilist.begin(), ilist.end()); 
    }

    // copy ctor 
    set(const set& rhs): _tree(rhs._tree)
    {

    }

    // move ctor 
    set(set&& rhs) noexcept :_tree(mystl::move(rhs._tree))
    {

    }

    // copy assign 
    set& operator=(const set& rhs)
    {
        _tree = rhs._tree;
        return *this; 
    }

    // move assign 
    set& operator= (set&& rhs)
    {
        // ? 
        _tree = mystl::move(rhs._tree); 
        return *this; 
    }

    set& operator= (std::initializer_list<value_type> ilist)
    {
        _tree.clear(); 
        _tree.insert_unique(ilist.begin(), ilist.end());
        return *this; 
    }

    // 
    key_compare             key_comp()      const { return _tree.key_comp(); } 
    value_compare           value_comp()    const { return _tree.key_comp(); } 
    allocator_type          get_allocator()    const { return _tree.get_allocator(); } 

    // iterator 

    iterator            begin()                     noexcept 
    { return _tree.begin(); } 
    const_iterator      begin()             const noexcept 
    { return _tree.begin(); } 
    iterator            end()                       noexcept 
    { return _tree.end(); } 
    const_iterator      end()               const noexcept 
    { return _tree.end(); } 
    
    reverse_iterator            rbegin()            noexcept 
    { return reverse_iterator(end()); } 
    const_reverse_iterator      rbegin()    const noexcept 
    { return const_reverse_iterator(end()); } 
    reverse_iterator            rend()              noexcept 
    { return reverse_iterator(begin()); } 
    const_reverse_iterator      rend()      const noexcept 
    { return const_reverse_iterator(begin()); } 

    const_iterator          cbegin()            const noexcept 
    { return begin(); } 
    const_iterator          cend()              const noexcept 
    { return end(); } 
    const_reverse_iterator  crbegin()           const noexcept 
    { return rbegin();} 
    const_reverse_iterator  crend()             const noexcept 
    { return rend(); } 

    // capacity 
    bool            empty()     const noexcept { return _tree.empty(); } 
    size_type       size()      const noexcept { return _tree.size(); } 
    size_type       max_size()  const noexcept { return _tree.max_size();} 

    // insert / erase  
    template <typename ...Args>  
    pair <iterator, bool> emplace (Args&& ...args) 
    {
        return _tree.emplace_unique(mystl::forward<Args>(args)...); 
    }

    template <typename ...Args>  
    iterator emplace_hint(iterator hint, Args&& ... args) 
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

    // range insert 
    template <typename InputIter>   
    void insert(InputIter first, InputIter last)
    {
        _tree.insert_unique(first, last); 
    }

    // erase 
    void        erase(iterator position)        { _tree.erase(position); } 
    size_type   erase(const key_type& key)      { return _tree.erase_unique(key); }
    void        erase(iterator first, iterator last) { return _tree.erase(first, last); } 

    // clear 
    void        clear()                         { _tree.clear(); } 

    // set operations 
    iterator            find(const key_type& key) { return _tree.find(key);} 
    const_iterator      find(const key_type& key) const { return _tree.find(key);} 

    size_type           count(const key_type& key) { return _tree.count_unique(key);} 
    
    iterator            lower_bound(const key_type& key) { return _tree.lower_bound(key); } 
    const_iterator      lower_bound(const key_type& key) const { return _tree.lower_bound(key); }

    iterator            upper_bound(const key_type& key) { return _tree.upper_bound(key); } 
    const_iterator      upper_bound(const key_type& key) const { return _tree.upper_bound(key); }

    // equal_range 
    pair<iterator, iterator>  
    equal_range(const key_type& key)
    { return _tree.equal_range_unique(key); } 

    pair<const_iterator, const_iterator> 
    equal_range(const key_type& key) const 
    { return _tree.equal_range_unique(key); } 

    // swap 
    void swap(set& rhs) noexcept 
    { _tree.swap(rhs._tree); } 

public:  
    // friend  
    friend bool operator== (const set& lhs, const set& rhs) { return lhs._tree == rhs._tree; } 
    friend bool operator<  (const set& lhs, const set& rhs) { return lhs._tree < rhs._tree; } 
}; 

// relational operator oveloading 
template <typename Key, typename Compar>  
bool operator== (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs) 
{
    return lhs == rhs; 
}

template <typename Key, typename Compar> 
bool operator!= (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs) 
{
    return !(lhs == rhs);
}
template <typename Key, typename Compar>  
bool operator< (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs)
{
    return lhs < rhs; 
}

template <typename Key, typename Compar>  
bool operator> (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs)
{
    return rhs < lhs; 
}

template <typename Key, typename Compar>  
bool operator <= (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs)
{
    return !(rhs < lhs); 
}

template <typename Key, typename Compar>  
bool operator >= (const set<Key, Compar>& lhs, const set<Key, Compar>& rhs)
{
    return !(lhs < rhs); 
}

// overloading generic swap 
template <typename Key, typename Compar>  
void swap(set<Key, Compar>& lhs, set<Key, Compar>& rhs) noexcept 
{
    lhs.swap(rhs); 
}

// multiset 
template <typename Key, typename Compar = mystl::less<Key>>  
class multiset 
{
public: 
    typedef Key             key_type; 
    typedef Key             value_type; 
    typedef Compar          key_compare; 
    typedef Compar          value_compare; 

private:  
    typedef mystl::rb_tree<value_type, key_compare>         base_type; 
    base_type                                               _tree; 

public:  
    // triats 
    typedef typename base_type::node_type               node_type; 
    typedef typename base_type::const_pointer           pointer; 
    typedef typename base_type::const_pointer           const_pointer; 
    typedef typename base_type::const_reference         reference; 
    typedef typename base_type::const_reference         const_reference; 
    typedef typename base_type::const_iterator          iterator; 
    typedef typename base_type::const_iterator          const_iterator; 
    typedef typename base_type::const_reverse_iterator  reverse_iterator;
    typedef typename base_type::const_reverse_iterator  const_reverse_iterator; 
    typedef typename base_type::size_type               size_type; 
    typedef typename base_type::difference_type         difference_type; 
    typedef typename base_type::allocator_type          allocator_type; 

public:  
    // ctors 
    multiset() = default; 
    // range ctor 
    template <typename InputIter>  
    multiset(InputIter first, InputIter last): _tree() 
    {
        _tree.insert_multi(first, last); 
    }
    // initialize list 
    multiset(std::initializer_list<value_type> ilist): _tree() 
    {   
        _tree.insert_multi(ilist.begin(), ilist.end()); 
    }

    // copy ctor 
    multiset(const multiset& rhs): _tree(rhs._tree)
    {

    }
    // move ctor 
    multiset(multiset&& rhs) noexcept: _tree(mystl::move(rhs._tree))
    {

    }

    // copy assign 
    multiset& operator= (const multiset& rhs)
    {
        _tree = rhs._tree; 
        return *this; 
    }

    // move assign 
    multiset& operator= (multiset&& rhs)
    {
        // _tree.clear? 
        _tree = mystl::move(rhs._tree); 
        return *this; 
    }

    // initialized list assign 
    multiset& operator= (std::initializer_list<value_type> ilist)
    {
        _tree.clear();
        _tree.insert_multi(ilist.begin(), ilist.end()); 
        return *this; 
    }

    key_compare         key_comp()                  const { return _tree.key_comp(); } 
    value_compare       value_comp()                const { return _tree.key_comp(); } 
    allocator_type      get_allocator()             const { return _tree.get_allocator(); } 

    // iterator 
    iterator            begin()                     noexcept 
    { return _tree.begin(); } 
    const_iterator      begin()                     const noexcept 
    { return _tree.begin(); } 
    iterator            end()                       noexcept 
    { return _tree.end(); } 
    const_iterator      end()                       const noexcept 
    { return _tree.end(); } 

    // reverse_iterator  
    reverse_iterator            rbegin()            noexcept 
    { return reverse_iterator(end());} 
    const_reverse_iterator      rbegin()            const noexcept 
    { return const_reverse_iterator(end()); } 
    reverse_iterator            rend()              noexcept 
    { return reverse_iterator(begin()); } 
    const_reverse_iterator      rend()              const noexcept 
    { return const_reverse_iterator(begin()); } 

    // const_iterator  
    const_iterator              cbegin()            const noexcept 
    { return begin(); } 
    const_iterator              cend()              const noexcept 
    { return end(); } 
    const_reverse_iterator      crbegin()           const noexcept 
    { return rbegin(); } 
    const_reverse_iterator      crend()             const noexcept 
    { return rend(); }     

    // capacity 
    bool        empty()     const noexcept { return _tree.empty(); } 
    size_type   size()      const noexcept { return _tree.size(); } 
    size_type   max_size()  const noexcept { return _tree.max_size(); } 


    // insert and erase 

    template <typename ...Args>  
    iterator emplace (Args&& ...args) 
    {
        return _tree.emplace_multi(mystl::forward<Args>(args)...); 
    }

    template <typename ...Args>  
    iterator emplace_hint(iterator hint, Args&& ...args) 
    {
        return _tree.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...); 
    }

    iterator insert (const value_type& value)
    { 
        return _tree.insert_multi(value); 
    }

    iterator  insert(value_type&& value) 
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

    // erase 
    void        erase(iterator position)            {_tree.erase(position); } 
    size_type   erase(const key_type& key)          { return _tree.erase_multi(key); } 
    void        erase(iterator first, iterator last) {_tree.erase(first, last); } 

    // clear 
    void        clear() {_tree.clear(); }

    // search operation 

    iterator        find(const key_type& key)       { return _tree.find(key); } 
    const_iterator  find(const key_type& key) const { return _tree.find(key); } 

    size_type       count(const key_type& key) const { return _tree.count_multi(key); } 

    iterator        lower_bound(const key_type& key)        { return _tree.lower_bound(key); } 
    const_iterator  lower_bound(const key_type& key) const  { return _tree.lower_bound(key); } 

    pair<iterator, iterator> equal_range(const key_type& key)
    { return _tree.equal_range_multi(key); } 
    pair<const iterator, const iterator>
    equal_range(const key_type& key) const 
    {
        return _tree.equal_range_multi(key); 
    }

    // swap 
    void swap(multiset& rhs) noexcept 
    {
        _tree.swap(rhs._tree); 
    } 

public:
    friend bool operator== (const multiset& lhs, const multiset& rhs)
    {
        return lhs._tree == rhs._tree; 
    }
    friend bool operator < (const multiset& lhs, const multiset& rhs)
    {
        return lhs._tree < rhs._tree; 
    }
}; 

// relational operators 
template <typename Key, typename Compar>  
bool operator == (const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs)
{
    return lhs == rhs; 
}

template <typename Key, typename Compar> 
bool operator != (const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs) 
{
    return !(lhs == rhs); 
}

template <typename Key, typename Compar>  
bool operator < (const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs) 
{
    return lhs < rhs; 
}

template <typename Key, typename Compar>  
bool operator > ( const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs)
{
    return rhs < lhs; 
}

template <typename Key, typename Compar>  
bool operator <=(const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs)
{
    return !(rhs < lhs); 
}

template <typename Key, typename Compar>  
bool operator >= (const multiset<Key, Compar>& lhs, const multiset<Key, Compar>& rhs)
{
    return !(lhs < rhs); 
}


// overloading generic swap 
template <typename Key, typename Compar>  
void swap(multiset <Key, Compar>& lhs, multiset<Key, Compar>& rhs) noexcept 
{
    lhs.swap(rhs); 
}
}   // end of namespace mystl 
#endif // !SET_H_