#include <gtest/gtest.h> 
#include "../include/util.h"
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
using namespace mystl;
 
struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};
 
class B {
public:
    template<class T1, class T2, class T3>
    B(T1&& t1, T2&& t2, T3&& t3) :
        a1_{mystl::forward<T1>(t1)},
        a2_{mystl::forward<T2>(t2)},
        a3_{mystl::forward<T3>(t3)}
    {
    }
 
private:
    A a1_, a2_, a3_;
};
 
template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(mystl::forward<U>(u)));
}
 
template<class T, class... U>
std::unique_ptr<T> make_unique2(U&&... u)
{
    return std::unique_ptr<T>(new T(mystl::forward<U>(u)...));
}
 
TEST(test2, forward)
{   
    auto p1 = make_unique1<A>(2); // rvalue
    int i = 1;
    auto p2 = make_unique1<A>(i); // lvalue
 
    std::cout << "B\n";
    auto t = make_unique2<B>(2, i, 3);
}


TEST(test1, remove_reference)
{
    std::string str = "Salut";
    std::vector<std::string> v;
 
    // uses the push_back(const T&) overload, which means 
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is " << std::quoted(str) << '\n';
 
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(mystl::move(str));
    std::cout << "After move, str is " << quoted(str) << '\n';
 
    std::cout << "The contents of the vector are { " << quoted(v[0])
                                             << ", " << quoted(v[1]) << " }\n";

    EXPECT_EQ(v[0], v[1]);
    EXPECT_EQ(str, ""); 
}

TEST(test3, swap)
{
    std::string a = "1";
    std::string b = "2"; 

    swap(a, b); 

    EXPECT_EQ(a, "2");     
    EXPECT_EQ(b, "1");
}

TEST(test4, rangeswap)
{
    std::vector<int> foo (5,10);        // foo: 10 10 10 10 10
    std::vector<int> bar (5,33);        // bar: 33 33 33 33 33

    std::swap_ranges(foo.begin()+1, foo.end()-1, bar.begin());

    // print out results of swap:
    std::cout << "foo contains:";
    for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    std::cout << "bar contains:";
    for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    // 10 33 33 33 10 
    // 10 10 10 33 33 
}


template <class Os, class T>
Os& operator<<(Os& os, const std::vector<T>& v) {
    os << "{";
    for (std::size_t t = 0; t != v.size(); ++t)
        os << v[t] << (t+1 < v.size() ? "," : "");
    return os << "}";
}
 
template <class Os, class U1, class U2>
Os& operator<<(Os& os, const mystl::pair<U1, U2>& pair) {
    return os << ":{ " << pair.first << ", " << pair.second << " } ";
}
 
TEST(test5, pair1)
{
    mystl::pair<int, std::vector<int>> p{ 1, {2} }, q{ 2, {5,6} };
    std::cout << std::setw(23) << std::left
              << "(0) p = q;" << "p" << p << "   q" << q << '\n';

    p = q; // (1) operator=( const pair& other );
    std::cout << std::setw(23) << std::left
              << "(1) p = q;" << "p" << p << "   q" << q << '\n';
 
    mystl::pair<short, std::vector<int>> r{ 4, {7,8,9} };
    p = r; // (2) operator=( const pair<U1,U2>& other );
    std::cout << std::setw(23)
              << "(2) p = r;" << "p" << p << " r" << r << '\n';
 
    p = mystl::pair<int, std::vector<int>>{ 3, {4} };
    p = std::move(q); // (3) operator=( pair&& other );
    std::cout << std::setw(23)
              << "(3) p = std::move(q);" << "p" << p << "   q" << q << '\n';
 
    p = mystl::pair<int, std::vector<int>>{ 5, {6} };
    p = std::move(r); // (4) operator=( pair<U1,U2>&& other );
    std::cout << std::setw(23)
              << "(4) p = std::move(r);" << "p" << p << " r" << r << '\n';
}

/*
(1) p = q;             p:{ 2, {5,6} }    q:{ 2, {5,6} } 
(2) p = r;             p:{ 4, {7,8,9} }  r:{ 4, {7,8,9} } 
(3) p = std::move(q);  p:{ 2, {5,6} }    q:{ 2, {} } 
(4) p = std::move(r);  p:{ 4, {7,8,9} }  r:{ 4, {} } 
*/

template<typename T1, typename T2> 
void display_pair(const pair<T1, T2>& p)
{
    std::cout <<"{" << p.first<<", "<<p.second << "}" << std::endl; 
}


TEST(test6, pair2)
{
    using mystl::pair; 
    using std::cout; using std::endl; 


    pair<int, int> p = {1,2};
    pair<int, int> q = {1,2};
    pair<int, int> r = {3,4}; 

    EXPECT_EQ(p == q, true); 
    EXPECT_EQ(p != r, true);
    EXPECT_EQ(p < r, true); 
    r = pair<int, int>{1,3}; 
    EXPECT_EQ(p < r, true); 
    EXPECT_EQ(p <=r, true);

    swap(r, q);
    EXPECT_EQ(p == r, true); 
    
    display_pair(p);
    display_pair(q);
    display_pair(r);
    q.swap(p); 
    display_pair(p);
    display_pair(q);


    auto ap = make_pair<int, int>(1,2);
    auto bp = make_pair<int, int>(1,2); 


    EXPECT_EQ(ap == bp, true);

}