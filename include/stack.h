// -- stack.h deque的容器适配器
#ifndef STACK_H_  
#define STACK_H_  

#include "deque.h" 

namespace mystl 
{
template <typename T, typename Container = mystl::deque<T>> 
class stack  
{
public: 
    typedef Container                           container_type; 
    typedef typename Container::value_type      value_type; 
    typedef typename Container::size_type       size_type; 
    typedef typename Container::reference       reference; 
    typedef typename Container::const_reference const_reference; 

    static_assert(std::is_same<T, value_type>::value, 
        "the value_type of Container should be same with T"); 

private: 
    container_type _c; 

public: 
    // ctors 
    stack() = default; 
    
    explicit stack(size_type n): _c(n) {} 
    
    stack(size_type n, const value_type& value): _c(n, value) {} 

    template<typename InputIter>
    stack(InputIter first, InputIter last): _c(first, last) {}  
    // initializer_list 
    stack(std::initializer_list<T> initlist): _c(initlist.begin(), initlist.end()) {} 
    // base_container 
    stack(const Container& c): _c(c) {} 
    // base_container (move ctor) 
    stack(Container&& c) noexcept (std::is_nothrow_move_constructible<Container>::value):
        _c(mystl::move(c)) {}  
    // copy ctor 
    stack(const stack& rhs): _c(rhs._c) {} 
    // move ctor 
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value): 
        _c(mystl::move(rhs._c)) {}  

    stack& operator=(const stack& rhs) 
    {
        _c = rhs._c; 
        return *this; 
    }

    stack& operator=(stack&&rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    {
        _c = std::move(rhs._c); 
        return *this; 
    }

    stack& operator= (std::initializer_list<T> initlist) 
    {
        _c = initlist; 
        return *this; 
    }

    ~stack() = default; 

    // 元素访问
    reference       top()   {return _c.back(); } 
    const_reference top()   const {return _c.back(); } 

    // capacity 
    bool empty() const noexcept {return _c.empty(); } 
    size_type size() const noexcept {return _c.size(); } 

    // modification 
    template <typename... Args>  
    void emplace(Args&&... args) 
    {
        _c.emplace_back(mystl::forward<Args>(args)...); 
    }

    void push(const value_type& value)
    {
        _c.push_back(value); 
    }

    void push(value_type&& value)
    {
        _c.push_back(mystl::move(value)); 
    }

    void pop() {_c.pop_back(); } 

    void clear() 
    {
        while(!empty()) pop(); 
    }

    void swap(stack&& rhs) noexcept(noexcept(mystl::swap(_c, rhs._c))) 
    {
        mystl::swap(_c, rhs._c); 
    }

public: 
    friend bool operator== (const stack& lhs, const stack& rhs) 
    {
        return lhs._c == rhs._c; 
    }

    friend bool operator< (const stack& lhs, const stack& rhs) 
    {
        return lhs._c < rhs._c; 
    }
}; 

// relational opearator overloading  
template <typename T, typename Container> 
bool operator== (const stack<T, Container>& lhs, const stack<T, Container>& rhs) 
{
    return lhs == rhs; 
}

template <typename T, typename Container> 
bool operator< (const stack<T, Container>& lhs, const stack<T, Container>& rhs) 
{
    return lhs < rhs; 
}

template <typename T, typename Container> 
bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs) 
{
    return !(lhs == rhs); 
}

template <typename T, typename Container> 
bool operator> (const stack<T, Container>& lhs, const stack<T, Container>& rhs) 
{
    return rhs < lhs; 
}

template <typename T, typename Container> 
bool operator <= (const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(rhs < lhs); 
}

template <typename T, typename Container>  
bool operator>= (const stack<T, Container>& lhs, const stack<T, Container>& rhs) 
{
    return !(lhs < rhs); 
}

// overloading generic swap 
template<typename T, typename Container> 
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}




}
#endif // !STACK_H_  
