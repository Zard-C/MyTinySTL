// -- queue.h: queue 和 priority_queue 
#ifndef QUEUE_H_
#define QUEUE_H_  

#include "deque.h"
#include "vector.h"
#include <functional> 
#include "heap_algo.h" 

namespace mystl 
{
// queue   
template <typename T, typename BaseContainer = mystl::deque<T>>  
class queue 
{
public:  
    typedef BaseContainer                                   container_type; 
    typedef typename BaseContainer::value_type              value_type; 
    typedef typename BaseContainer::size_type               size_type; 
    typedef typename BaseContainer::reference               reference; 
    typedef typename BaseContainer::const_reference         const_reference; 

    static_assert(std::is_same<T, value_type>::value, 
                    "the value_type of Container should be same with T"); 

private:  
    container_type _c;      

public:  
    // ctors 
    queue() = default; 

    explicit queue(size_type n): _c(n) {}  

    queue(size_type n, const value_type& value): _c(n, value) {} 

    template <typename InputIter>   
    queue(InputIter first, InputIter last): _c(first, last) 
    {

    }       

    queue(std::initializer_list<T> initlist): _c(initlist.begin(), initlist.end())
    {

    }

    queue(const BaseContainer& lc): _c(lc) {}  

    queue(BaseContainer&& rc) noexcept(std::is_nothrow_move_constructible<BaseContainer>::value):  
        _c(mystl::move(rc)) {}  
    
    // copy ctor 
    queue(const queue& rhs): _c(rhs._c) {}  

    // move ctor 
    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<BaseContainer>::value)
                        :_c(mystl::move(rhs)) {} 

    // operator=  
    queue& operator= (const queue& rhs) 
    {
        _c = rhs._c; 
        return *this; 
    }

    queue& operator= (queue&& rhs) noexcept(std::is_nothrow_move_assignable<BaseContainer>::value)
    {
        _c = mystl::move(rhs._c); 
    }

    ~queue() = default; 

    // visitor 
    reference front()               {return _c.front(); } 
    const_reference front()   const {return _c.front(); } 
    reference back()                {return _c.back();  }
    const_reference back()    const {return _c.back();  } 

    // size 
    bool empty()        const noexcept  {return _c.empty(); } 
    size_type size()    const noexcept  {return _c.size();  }

    // emplace 
    template <typename... Args>  
    void emplace(Args... args) 
    {
        _c.emplace_back(mystl::forward<Args>(args)...); 
    }

    void push(const value_type& value)
    {
        _c.push_back(value); 
    }

    void push(value_type&& value) 
    {
        _c.emplace_back(mystl::move(value)); 
    }

    void pop() 
    {
        _c.pop_front();  
    }

    void clear() 
    {
        while(!empty()) 
        {
            pop(); 
        }
    }

    void swap(queue& rhs) noexcept(noexcept(mystl::swap(_c, rhs._c))) 
    {
        mystl::swap(_c, rhs._c); 
    }
public: 
    friend bool operator== (const queue& lhs, const queue& rhs)
    {
        return lhs._c == rhs._c; 
    }

    friend bool operator< (const queue& lhs, const queue& rhs) 
    {
        return lhs._c < rhs._c; 
    }
};

// overload relational operators 
template <typename T, typename BaseContainer> 
bool operator==(const queue<T, BaseContainer>& lhs, const queue<T, BaseContainer>& rhs) 
{
    return lhs == rhs; 
}

template <typename T, typename BaseContainer>  
bool operator<(const queue<T, BaseContainer>& lhs, const queue<T, BaseContainer>& rhs) 
{
    return lhs < rhs; 
}

template <typename T, typename BaseContainer>  
bool operator <= (const queue<T, BaseContainer>& lhs, const queue<T, BaseContainer>& rhs)
{
    return !(rhs < lhs); 
}

template <typename T, typename BaseContainer>  
bool operator >= (const queue<T, BaseContainer>& lhs, const queue<T, BaseContainer>& rhs)
{
    return !(lhs < rhs); 
}

// priority_queue 
// 默认使用vector作为底层容器
// 通过Compare 给出比较规则，默认以less<T> 作为比较方式
template<typename T, typename BaseContainer= mystl::vector<T>, 
            typename Compar = std::less<typename BaseContainer::value_type>>
class priority_queue      
{
    typedef BaseContainer                                       container_type; 
    typedef Compar                                              value_compare; 
    // 
    typedef typename BaseContainer::value_type                  value_type; 
    typedef typename BaseContainer::size_type                   size_type; 
    typedef typename BaseContainer::reference                   reference; 
    typedef typename BaseContainer::const_reference             const_reference; 

    static_assert(std::is_same<T, value_type>::value, 
        "the value_type of BaseContainer should be same with T"); 

private: 
    container_type  _c; 
    value_compare   _comp;  

public:  
    // ctors 
    priority_queue() = default; 

    // 
    priority_queue(const Compar& cmpfn):_c(), _comp(cmpfn) {}  

    explicit priority_queue(size_type n): _c(n) 
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    } 

    priority_queue(size_type n, const value_type value): _c(n, value)
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    template<typename InputIter>  
    priority_queue(InputIter first, InputIter last): _c(first, last) 
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    priority_queue(std::initializer_list<T> initlist): _c(initlist) 
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    priority_queue(const BaseContainer& container): _c(container)
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    priority_queue(BaseContainer&& container): _c(mystl::move(container))
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    // copy ctor 
    priority_queue(const priority_queue& rhs):_c(rhs._c), _comp(_c.comp)
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }

    // move ctor 
    priority_queue(priority_queue&& rhs):_c(mystl::move(rhs._c)), _comp(rhs.comp) 
    {
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
    }
    // copy assign 
    priority_queue& operator= (const priority_queue& rhs) 
    {
        _c = rhs._c; 
        _comp = rhs._comp; 
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
        return *this; 
    } 
    // move assign 
    priority_queue& operator=(priority_queue&& rhs)
    {
        _c = mystl::move(rhs._c); 
        _comp = rhs._comp; 
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
        return *this; 
    }

    // range assign 
    priority_queue& operator= (std::initializer_list<T> initlist)
    {
        _c = initlist; 
        _comp = value_compare(); 
        mystl::make_heap(_c.begin(), _c.end(), _comp); 
        return *this; 
    }

    ~priority_queue() = default; 

public: 
    // visit 
    const_reference top() const {return _c.front(); } 

    // capacity 
    bool empty()            const noexcept {return _c.empty(); } 
    size_type   size()      const noexcept {return _c.size(); }

    template <typename... Args>  
    void emplace(Args&& ... args) 
    {
        _c.emplace_back(mystl::forward<Args>(args)...); 
        mystl::push_heap(_c.begin(), _c.end(), _comp); 
    }

    void push(const value_type& value)
    {
        _c.push_back(value); 
        mystl::push_heap(_c.begin(), _c.end(), _comp); 
    }

    void push(value_type&& value)
    {
        _c.push_back(mystl::move(value)); 
        mystl::push_heap(_c.begin(), _c.end(), _comp); 
    }

    void pop() 
    {
        mystl::pop_heap(_c.begin(), _c.end(), _comp); 
        _c.pop_back(); 
    }

    void clear() 
    {
        while(!empty()) pop(); 
    }
}; 

// overload relational operators 
template <typename T, typename Container, typename Compare>  
bool 
operator== (priority_queue<T, Container, Compare>& lhs, 
            priority_queue<T, Container, Compare>& rhs)
{
    return lhs == rhs; 
}

template <typename T, typename Container, typename Compare>  
bool 
operator!= (priority_queue<T, Container, Compare>& lhs, 
            priority_queue<T, Container, Compare>& rhs)
{
    return lhs != rhs; 
}

// overload generic swap 
template <typename T, typename Container, typename Compare>  
void swap(priority_queue<T, Container, Compare>& lhs, 
        priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs); 
}

} // end of namespace mystl 
#endif // !QUEUE_H_