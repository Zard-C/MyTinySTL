// -- iterator.h 实现迭代器
#ifndef ITERATOR_H_
#define ITERATOR_H_ 

#include <cstddef> 
#include "type_traits.h"
#include <iterator> 

namespace mystl
{

// iterator_tags 
#if 0 
struct input_iterator_tag{}; 
struct output_iterator_tag{};  
struct forward_iterator_tag:public input_iterator_tag {}; 
struct bidirectional_iterator_tag: public forward_iterator_tag {}; 
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
#endif 

typedef std::input_iterator_tag         input_iterator_tag; 
typedef std::output_iterator_tag        output_iterator_tag; 
typedef std::forward_iterator_tag       forward_iterator_tag; 
typedef std::bidirectional_iterator_tag bidirectional_iterator_tag; 
typedef std::random_access_iterator_tag random_access_iterator_tag; 


// iterator template 
template <typename Category, typename T, typename Distance = ptrdiff_t,
            typename Pointer = T*, typename Reference = T&> 
struct iterator 
{
    typedef Category            iterator_category; 
    typedef T                   value_type; 
    typedef Pointer             pointer; 
    typedef Reference           reference; 
    typedef Distance            difference_type; 
}; 

// iterator traits 
// usage: has_iterator_cat<T>::value 
template <typename T> 
struct has_iterator_cat
{
private: 
    struct two {char a; char b;}; 
    template <typename U> 
    static two test(...); // size = 2*sizeof(char)
    template <typename U> 
    static char test(typename U::iterator_category* = 0); // size = sizeof(char) 
public: 
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(char); 
};


template <typename Iterator, bool> 
struct iterator_traits_impl {};

template <typename Iterator> 
struct iterator_traits_impl <Iterator, true> 
{
    typedef typename Iterator::iterator_category    iterator_category; 
    typedef typename Iterator::value_type           value_type; 
    typedef typename Iterator::pointer              pointer; 
    typedef typename Iterator::reference            reference; 
    typedef typename Iterator::difference_type      difference_type; 
};

// 
template <typename Iterator, bool> 
struct iterator_traits_helper {}; 

template <typename Iterator>
struct iterator_traits_helper <Iterator, true>:     // 为了兼容STL进行测试，比较的使用std::input_iterator_tag
public iterator_traits_impl<Iterator, 
std::is_convertible<typename Iterator::iterator_category, std::input_iterator_tag>::value ||
std::is_convertible<typename Iterator::iterator_category, std::output_iterator_tag>::value> 
{

}; 

// iterator type_traits 
template <typename Iterator> 
struct iterator_traits : public iterator_traits_helper <Iterator, has_iterator_cat<Iterator>::value>  
{}; 


// 原生指针的特化版本
template <typename T> 
struct iterator_traits<T*> 
{
    typedef random_access_iterator_tag      iterator_category; 
    typedef T                               value_type; 
    typedef T*                              pointer; 
    typedef T&                              reference; 
    typedef ptrdiff_t                       difference_type; 
                                            
}; 

template <typename T> 
struct iterator_traits<const T*> 
{
    typedef random_access_iterator_tag      iterator_category; 
    typedef T                               value_type; 
    typedef const T*                        pointer; 
    typedef const T&                        reference; 
    typedef ptrdiff_t                       difference_type; 
}; 

// Is T has iterator_cat_of U ? 
template <typename T, typename U, bool = has_iterator_cat<iterator_traits<T>>::value> 
struct has_iterator_cat_of: public m_bool_constant<
std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value> 
{}; 

// 
template <typename T, typename U> 
struct has_iterator_cat_of<T, U, false>: public m_false_type {}; 

template <typename I> 
struct is_input_iterator: public has_iterator_cat_of<I, input_iterator_tag> {}; 

template <typename I> 
struct is_output_iterator: public has_iterator_cat_of<I, output_iterator_tag> {}; 

template <typename I> 
struct is_forward_iterator: public has_iterator_cat_of<I, forward_iterator_tag> {}; 

template <typename I> 
struct is_bidirectional_iterator:public has_iterator_cat_of<I, bidirectional_iterator_tag> {}; 

template <typename I> 
struct is_random_access_iterator: public has_iterator_cat_of<I, random_access_iterator_tag> {}; 


template <typename I> 
struct is_iterator: public m_bool_constant <is_input_iterator<I>::value ||
is_output_iterator<I>::value> 
{}; 

// 获取迭代器的种类
template <typename I> 
typename iterator_traits<I>::iterator_category
iterator_category(const I&)
{
    typedef typename iterator_traits<I>::iterator_category Category; 
    return Category(); 
}

// 获取迭代器的difference_type 
template<typename I> 
typename iterator_traits<I>::difference_type* 
distance_type(const I&)
{
    return static_cast<typename iterator_traits<I>::difference_type*>(0); 
}

// 萃取迭代器的value_type 
template <typename I> 
typename iterator_traits<I>::value_type* 
value_type(const I&)
{
    return static_cast<typename iterator_traits<I>::value_type*>(0); 
}

// distance_dispatch 
template <typename InputIterator> 
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0; 
    while(first != last)
    {
        ++first; ++n; 
    }
    return n; 
}

template <typename RandomIterator> 
typename iterator_traits<RandomIterator>::difference_type 
distance_dispatch(RandomIterator first, RandomIterator last, random_access_iterator_tag)
{
    return last - first; 
}

// distance 
template <typename InputIterator> 
typename iterator_traits<InputIterator>::difference_type 
distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}

// advance_dispatch
template <typename InputIterator, typename Distance> 
void advance_dispatch(InputIterator& ite, Distance n, input_iterator_tag)
{
    while(n--) ++ite;
}

//
template <typename BidirectionalIterator, typename Distance> 
void advance_dispatch(BidirectionalIterator & ite, Distance n, bidirectional_iterator_tag)
{
    if(n >= 0)
    {while(n--) ++ite;}
    else 
    {while(n++) --ite;}
}

// random iterator 
template<typename RandomIterator, typename Distance> 
void advance_dispatch(RandomIterator ite, Distance n, random_access_iterator_tag)
{
    ite +=n; 
}

// advance 
template <typename InputIterator, typename Distance> 
void advance(InputIterator& ite, Distance n)
{
    advance_dispatch(ite, n, iterator_category(ite)); 
}

// reverse_iterator 
// 反向迭代器，前进为后退，后退为前进，并注意哨兵位置
template<typename Iterator> 
class reverse_iterator
{
private: 
    Iterator current;   // 封装正向迭代器

public: 
    // for type_traits 
    typedef typename mystl::iterator_traits<Iterator>::iterator_category   iterator_category; 
    typedef typename iterator_traits<Iterator>::value_type          value_type; 
    typedef typename iterator_traits<Iterator>::difference_type     difference_type; 
    typedef typename iterator_traits<Iterator>::pointer             pointer; 
    typedef typename iterator_traits<Iterator>::reference           reference; 


    typedef Iterator                                                iterator_type; 
    typedef reverse_iterator<Iterator>                              self; 

public:  
    // ctor
    reverse_iterator(){} 
    explicit reverse_iterator(iterator_type i):current(i){} 
    // copy ctor
    reverse_iterator(const self& rhs):current(rhs.current){}

public:
    // 取出封装的正向迭代器
    iterator_type base() const {return current; }
    
    // 重载解引用操作符
    reference operator* () const
    {
        auto tmp = current; 
        return *(--tmp);
    }

    // 重载成员访问操作符
    pointer operator-> () const 
    {
        return &(operator*());
    }

    // 前进++ 变为 后退--
    self& operator++()
    {
        --current; 
        return *this; 
    }

    self operator++(int)
    {
        self tmp = *this; 
        -- current;
        return tmp; 
    }

    // 后退-- 变为前进
    self& operator--() 
    {
        ++current; 
        return *this; 
    }

    self operator--(int)
    {
        self tmp = *this; 
        ++current; 
        return tmp; 
    }

    self operator+(difference_type n) const 
    {
        return self(current - n); 
    }

    self operator+=(difference_type n)
    {
        current -= n; 
        return *this; 
    }

    self operator-(difference_type n) const
    {
        return self (current +n); 
    }

    self operator-=(difference_type n) 
    {
        current += n; 
        return *this; 
    }

    // 下标访问运算符
    reference operator[](difference_type n) const
    {
        return *(this + n); 
    }
};

// 重载 operator-  ???? 
template <typename Iterator> 
typename reverse_iterator<Iterator>::difference_type
operator- (const reverse_iterator<Iterator>& lhs, 
            const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() - lhs.base(); 
}

// 重载比较操作符
template<typename Iterator> 
bool operator== (const reverse_iterator<Iterator>& lhs, 
                 const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() == rhs.base(); 
}

template <typename Iterator> 
bool operator<(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() < lhs.base(); 
}

template <typename Iterator> 
bool operator!= (const reverse_iterator<Iterator>& lhs, 
                 const reverse_iterator<Iterator>& rhs)
{
    return !(lhs == rhs);
}

template <typename  Iterator> 
bool operator >(const reverse_iterator<Iterator>& lhs, 
                const reverse_iterator<Iterator>& rhs)
{
    return rhs < lhs; // 使用已经重载的 < 
}

template< typename Iterator> 
bool operator >= (const reverse_iterator<Iterator>& lhs, 
                  const reverse_iterator<Iterator>& rhs)
{
    return !(rhs < lhs); 
}

template <typename Iterator> 
bool operator <= (const reverse_iterator<Iterator>& lhs, 
                  const reverse_iterator<Iterator>& rhs)
{
    return !(lhs < rhs); 
}

} // end of namespace 
#endif // !ITERATOR_H_