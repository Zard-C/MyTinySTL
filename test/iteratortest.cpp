#include "iterator.h"
#include <gtest/gtest.h> 
#include <iostream> 
#include <iterator> 
#include <vector> 
#include <list> 


template<long FROM, long TO>
class Range 
{
public:
    // member typedefs provided through inheriting from std::iterator
    class iterator: public mystl::iterator<
                        mystl::input_iterator_tag,   // iterator_category
                        long,                      // value_type
                        long,                      // difference_type
                        const long*,               // pointer
                        long                       // reference
                                      >
    {
        long num = FROM;
    public:
        explicit iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        reference operator*() const {return num;}
    };
    iterator begin() {return iterator(FROM);}
    iterator end() {return iterator(TO >= FROM? TO+1 : TO-1);}
};
 

TEST(test1, iteTest) 
{
    // std::find requires an input iterator
    auto range = Range<15, 25>();
    auto itr = std::find(range.begin(), range.end(), 18);
    std::cout << *itr << '\n'; // 18
 
    // Range::iterator also satisfies range-based for requirements
    for(long l : Range<3, 5>()) {
        std::cout << l << ' '; // 3 4 5
    }
    std::cout << '\n';
}


# if 0 
TEST(test2, randomIterator)
{
  std::vector<int> myvector;
  for (int i=0; i<10; i++) myvector.push_back(i);

  typedef std::vector<int>::iterator iter_type;
                                                         // ? 9 8 7 6 5 4 3 2 1 0 ?
  iter_type from (myvector.begin());                     //   ^
                                                         //         ------>
  iter_type until (myvector.end());                      //                       ^
                                                         //
  mystl::reverse_iterator<iter_type> rev_until (from);     // ^
                                                         //         <------
  mystl::reverse_iterator<iter_type> rev_from (until);     //                     ^

  std::cout << "myvector:";
  while (rev_from != rev_until)
    std::cout << ' ' << *rev_from++;
  std::cout << '\n';

} 

#endif 
template<class BidirIt>
void my_reverse(BidirIt first, BidirIt last)
{
    typename std::iterator_traits<BidirIt>::difference_type n = std::distance(first, last);
    --n;
    while(n > 0) {
        typename std::iterator_traits<BidirIt>::value_type tmp = *first;
        *first++ = *--last;
        *last = tmp;
        n -= 2;
    }
}
 
TEST(test3, reverseIterator)
{
    int  v[5]{1, 2, 3, 4, 5};
    my_reverse(v, v + 5);
    for (int n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
 
 # if 1
    std::list<int> l{1, 2, 3, 4, 5};
    my_reverse(l.begin(), l.end());
    for (auto n : l) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
 
 #endif 
    int a[] = {1, 2, 3, 4, 5};
    my_reverse(a, a+5);
    for (int i=0; i<5; ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << '\n';
 
//    std::istreambuf_iterator<char> i1(std::cin), i2;
//    my_reverse(i1, i2); // compilation error
 
}


template <typename T> 
struct foo: mystl::input_iterator_tag
{
    typedef input_iterator_tag  iterator_category; 
    typedef T                   value_type; 
    typedef T*                  pointer;
    typedef T&                  reference; 
    typedef ptrdiff_t           difference_type; 

    // 重载操作符啥的 实现pointer-like
}; 

template<typename T> 
struct bar
{

}; 

TEST(test4, has_iterator_cat)
{
    using std::cout; using std::endl; 
    using mystl::has_iterator_cat; 

    cout << has_iterator_cat<int>::value<<endl; 

    EXPECT_EQ(has_iterator_cat<int>::value, false); 

    cout << has_iterator_cat<foo<int>>::value << endl; 
    EXPECT_EQ(has_iterator_cat<foo<int>>::value, true);


    cout << has_iterator_cat<std::vector<int>>::value << endl;
    EXPECT_EQ(has_iterator_cat<std::vector<int>>::value, false); 
    cout << typeid(std::vector<int>::value_type).name() << endl;
    cout << typeid(std::vector<int>::pointer).name() << endl; 
    cout << typeid(std::vector<int>::reference).name()<< endl; 
    cout << typeid(std::vector<double>::difference_type).name() << endl;   
}
 
TEST(test5, iterator_traits_helper)
{
    using mystl::iterator_traits_helper; 

}

TEST(test6, iteartor_traits)
{
    int arr[] {1,2,3,4,5}; 
    using std::cout; using std::endl; 
    using mystl::iterator_traits; 
    cout << typeid(iterator_traits<int* >::iterator_category).name() << endl; 
    cout << typeid(iterator_traits<int* >::value_type).name() << endl; 
    cout << typeid(iterator_traits<int *>::pointer).name() << endl; 
    cout << typeid(iterator_traits<int*> ::reference).name() << endl; 
    cout << typeid(iterator_traits<int*>:: difference_type).name() << endl; 

        cout << typeid(iterator_traits<const int* >::iterator_category).name() << endl; 
    cout << typeid(iterator_traits<const int* >::value_type).name() << endl; 
    cout << typeid(iterator_traits<const int *>::pointer).name() << endl; 
    cout << typeid(iterator_traits<const int*> ::reference).name() << endl; 
    cout << typeid(iterator_traits<const int*>:: difference_type).name() << endl; 
}

TEST(test7, has_iterator_cat_of)
{
    using mystl::input_iterator_tag; 
    using mystl::random_access_iterator_tag; 
    using mystl::has_iterator_cat_of; 
    using std::cout; using std:: endl; 


    cout << has_iterator_cat_of<foo<int>, input_iterator_tag>::value << endl;
    cout << has_iterator_cat_of<int, input_iterator_tag>::value << endl; 
    cout << has_iterator_cat_of<int*, input_iterator_tag>::value << endl; 
    cout << has_iterator_cat_of<const int, input_iterator_tag>::value << endl; 
    cout << has_iterator_cat_of<const int*, input_iterator_tag>::value << endl; 

    // 自信点，原生指针should be random_access_iterator 
    cout << has_iterator_cat_of<const int*, random_access_iterator_tag>::value << endl;
    cout << has_iterator_cat_of<foo<int>, random_access_iterator_tag>::value << endl;

    EXPECT_EQ((has_iterator_cat_of<const int*, random_access_iterator_tag>::value), true); 
    EXPECT_EQ((has_iterator_cat_of<foo<int>, random_access_iterator_tag>::value), false);  
}

TEST(test8, getIteratorCategory)
{
    using mystl::iterator_category;
    using std::cout; using std::endl; 

    int *p = nullptr; 
    auto category = iterator_category(p); 
    cout << typeid(category).name() << endl; 
    
    auto cate2 = iterator_category(foo<int>()); 
    cout <<typeid(cate2).name() << endl; 
    

}

TEST(test9, getIteratorDifferenceType)
{    
    using mystl::distance_type; 
    using std::cout; using std::endl; 
    int * a = nullptr; 

    auto ptr = distance_type(a); 
    cout << typeid(ptr).name() << endl; 

    auto ptr2 = distance_type(foo<int>()); 
    cout << typeid(ptr2).name() << endl;    // ptrdiff_t 
}

TEST(test10, valuetype)
{
    using mystl::value_type;
    using std::cout; using std::endl; 


    int* a = nullptr; 
    auto ptr = value_type(a); 
    cout << typeid(ptr).name() << endl; 

    auto ptr2 = value_type(foo<double>()); 
    cout << typeid(ptr2).name() << endl; 

}

TEST(test11, distance)
{
    using mystl::distance;
    using std::cout; using std::endl; 

    int arr[] {1,2,3,4,5}; 

    auto diff = distance(arr, arr + 5); 

    cout << diff << endl; 

    foo<int> arrF[10]; 

    auto diff2 = distance (arrF, arrF + 10); // ptrdiff_t  类型

    cout << diff2 << endl; 

}