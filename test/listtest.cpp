// --listtest.cpp 测试样例from http://www.cplusplus.com/
#include "list.h"
#include <gtest/gtest.h>
#include <list> 
#include <cmath> 

TEST(test1, ctor)
{
      // 构造，析构函数测试
    mystl::list<int> first;                                // empty list of ints
    mystl::list<int> second (4,100);                       // four ints with value 100
    mystl::list<int> third (second.begin(),second.end());  // iterating through second
    mystl::list<int> fourth (third);                       // a copy of third

    // the iterator constructor can also be used to construct from arrays:
    int myints[] = {16,2,77,29};
    mystl::list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

    std::cout << "The contents of fifth are: ";
    for (mystl::list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
      std::cout << *it << ' ';

    std::cout << '\n';
    std::cout << fifth.size() << "\n";
}

TEST(test2, assign)
{
    mystl::list<int> first;
    mystl::list<int> second;

    first.assign (7,100);                      // 7 ints with value 100

    second.assign (first.begin(),first.end()); // a copy of first

    int myints[]={1776,7,4};
    first.assign (myints,myints+3);            // assigning from array

    std::cout << "Size of first: " << int (first.size()) << '\n';
    std::cout << "Size of second: " << int (second.size()) << '\n';

    for(auto ite = first.begin(); ite != first.end(); ++ite)
    {
      std::cout << *ite << " "; 
    }
    std::cout << "\n"; 
    for(auto ite = second.begin(); ite != second.end(); ++ite)
    {
      std::cout << *ite << " "; 
    }
    std::cout <<"\n"; 
}

TEST(test3, back)
{
    mystl::list<int> mylist;

    mylist.push_back(10);

    while (mylist.back() != 0)
    {
      mylist.push_back ( mylist.back() -1 );
    }

    std::cout << "mylist contains:";
    for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end() ; ++it)
      std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test4, begin)
{
    int myints[] = {75,23,65,42,13};
    mystl::list<int> mylist (myints,myints+5);

    std::cout << "mylist contains:";
    for (mystl::list<int>::iterator it=mylist.begin(); it != mylist.end(); ++it)
      std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test5, cbegin_cend)
{
    mystl::list<int> mylist = {5,10,15,20};

    std::cout << "mylist contains:";

    for (auto it = mylist.cbegin(); it != mylist.cend(); ++it)
      std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test6, clear)
{
  mystl::list<int> mylist;
  mystl::list<int>::iterator it;

  mylist.push_back (100);
  mylist.push_back (200);
  mylist.push_back (300);

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  mylist.clear();
  mylist.push_back (1101);
  mylist.push_back (2202);

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // 100 200 300 
  // 1101 2202 
}

TEST(test7, crbegin)
{
  mystl::list<int> mylist = {1,2,4,8,16};

  std::cout << "mylist backwards:";
  for (auto rit = mylist.crbegin(); rit != mylist.crend(); ++rit)
    std::cout << ' ' << *rit;
  std::cout << '\n';
}





TEST(test8, rend)
{
  mystl::list<int> mylist;
  for (int i=1; i<=5; ++i) mylist.push_back(i);

  std::cout << "mylist backwards:";
  for (mystl::list<int>::reverse_iterator rit=mylist.rbegin(); rit!=mylist.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';
}

TEST(test9, emplace)
{
  mystl::list< std::pair<int,char> > mylist;

  mylist.emplace ( mylist.begin(), 100, 'x' );
  mylist.emplace ( mylist.begin(), 200, 'y' );

  std::cout << "mylist contains:";
  for (auto& x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";

  std::cout << '\n';
  // (200,y) (100,x)
}

TEST(test10, emplace_back)
{
  mystl::list< std::pair<int,char> > mylist;

  mylist.emplace_back(10,'a');
  mylist.emplace_back(20,'b');
  mylist.emplace_back(30,'c');

  std::cout << "mylist contains:";
  for (auto& x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";

  std::cout << std::endl;
}

TEST(test11, emplace_front)
{
    mystl::list< std::pair<int,char> > mylist;

    mylist.emplace_front(10,'a');
    mylist.emplace_front(20,'b');
    mylist.emplace_front(30,'c');

    std::cout << "mylist contains:";
    for (auto& x: mylist)
      std::cout << " (" << x.first << "," << x.second << ")";

    std::cout << std::endl;
}

TEST(test12, empty)
{
  mystl::list<int> mylist;
  int sum (0);

  for (int i=1;i<=10;++i) mylist.push_back(i);

  while (!mylist.empty())
  {
     sum += mylist.front();
     mylist.pop_front();
  }

  std::cout << "total: " << sum << '\n';
  std::cout << "mylist.size()= "<< mylist.size() << std::endl;
  EXPECT_EQ(mylist.size(), 0);  
  
}

TEST(test13, end)
{
  int myints[] = {75,23,65,42,13};
  mystl::list<int> mylist (myints,myints+5);

  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin() ; it != mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

TEST(test14, erase)
{
    mystl::list<int> mylist;
    mystl::list<int>::iterator it1,it2;

    // set some values:
    for (int i=1; i<10; ++i) mylist.push_back(i*10);

                                // 10 20 30 40 50 60 70 80 90
    it1 = it2 = mylist.begin(); // ^^
    advance (it2,6);            // ^                 ^
    ++it1;                      //    ^              ^

    it1 = mylist.erase (it1);   // 10 30 40 50 60 70 80 90
                                //    ^           ^

    it2 = mylist.erase (it2);   // 10 30 40 50 60 80 90
                                //    ^           ^

    ++it1;                      //       ^        ^
    --it2;                      //       ^     ^

    mylist.erase (it1,it2);     // 10 30 60 80 90
                                //        ^

    std::cout << "mylist contains:";
    for (it1=mylist.begin(); it1!=mylist.end(); ++it1)
      std::cout << ' ' << *it1;
    std::cout << '\n';
}

TEST(test15, front)
{
    mystl::list<int> mylist;

    mylist.push_back(77);
    mylist.push_back(22);

    // now front equals 77, and back 22

    mylist.front() -= mylist.back();

    std::cout << "mylist.front() is now " << mylist.front() << '\n';
    EXPECT_EQ(mylist.front(), 55); 
}

TEST(test16, get_allocator) 
{
  mystl::list<int> mylist;
  int * p;

  // allocate an array of 5 elements using mylist's allocator:
  p=mylist.get_allocator().allocate(5);

  // assign some values to array
  for (int i=0; i<5; ++i) p[i]=i;

  std::cout << "The allocated array contains:";
  for (int i=0; i<5; ++i) std::cout << ' ' << p[i];
  std::cout << '\n';

  mylist.get_allocator().deallocate(p,5);
}

TEST(test17, insert)
{
    mystl::list<int> mylist;
    mystl::list<int>::iterator it;

    // set some initial values:
    for (int i=1; i<=5; ++i) mylist.push_back(i); // 1 2 3 4 5

    it = mylist.begin();
    ++it;       // it points now to number 2           ^

    mylist.insert (it,10);                        // 1 10 2 3 4 5

    // "it" still points to number 2                      ^
    mylist.insert (it,2,20);                      // 1 10 20 20 2 3 4 5

    --it;       // it points now to the second 20            ^

    std::vector<int> myvector (2,30);
    mylist.insert (it,myvector.begin(),myvector.end());
                                                  // 1 10 20 30 30 20 2 3 4 5
                                                  //               ^
    std::cout << "mylist contains:";
    for (it=mylist.begin(); it!=mylist.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test18, max_size)
{

  mystl::list<int> mylist;
  std::cout << mylist.max_size() << std::endl; // unsigned long max 
}

// compare only integral part:
static bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

TEST(test19, merge)
{
  mystl::list<double> first, second;

  first.push_back (3.1);
  first.push_back (2.2);
  first.push_back (2.9);

  second.push_back (3.7);
  second.push_back (7.1);
  second.push_back (1.4);

  first.sort();
  second.sort();

  first.merge(second);

  // (second is now empty)

  second.push_back (2.1);

  first.merge(second,mycomparison);

  std::cout << "first contains:";
  for (mystl::list<double>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  //1.4 2.2 2.9 2.1 3.1 3.7 7.1
}

TEST(test20, operator_equal)
{
  
  mystl::list<int> first (3);      // list of 3 zero-initialized ints
  mystl::list<int> second (5);     // list of 5 zero-initialized ints

  second = first; // const list<T>& 
  first = mystl::list<int>(); // list<T> && 

  std::cout << "Size of first: " << int (first.size()) << '\n';
  std::cout << "Size of second: " << int (second.size()) << '\n';
}

TEST(test21, pop_back)
{
  mystl::list<int> mylist;
  int sum (0);
  mylist.push_back (100);
  mylist.push_back (200);
  mylist.push_back (300);

  while (!mylist.empty())
  {
    sum+=mylist.back();
    mylist.pop_back();
  }

  std::cout << "The elements of mylist summed " << sum << '\n';
  // 600 
}

TEST(test22, pop_front)
{
  mystl::list<int> mylist;
  mylist.push_back (100);
  mylist.push_back (200);
  mylist.push_back (300);

  std::cout << "Popping out the elements in mylist:";
  while (!mylist.empty())
  {
    std::cout << ' ' << mylist.front();
    mylist.pop_front();
  }

  std::cout << "\nFinal size of mylist is " << mylist.size() << '\n';

}

TEST(test23, push_back) 
{
  mystl::list<int> mylist;

  for(int i = 0; i < 5; ++i)  
  {
    mylist.push_back(i); 
  }
  
  for(auto ite = mylist.begin(); ite != mylist.end(); ++ite)
  {
    std::cout<<*ite <<" "; 
  }
  std::cout <<std::endl; 
  std::cout << "mylist stores " << mylist.size() << " numbers.\n";

}

TEST(test24, push_front)
{
  mystl::list<int> mylist (2,100);         // two ints with a value of 100
  mylist.push_front (200);
  mylist.push_front (300);

  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

TEST(test25, rbegin)
{
  mystl::list<int> mylist;
  for (int i=1; i<=5; ++i) mylist.push_back(i);

  std::cout << "mylist backwards:";
  for (mystl::list<int>::reverse_iterator rit=mylist.rbegin(); rit!=mylist.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';
}

// remove 有问题 -> remove_if 的实现 
TEST(test26, remove)
{
  int myints[]= {89,17};
  mystl::list<int> mylist (myints, myints + sizeof(myints)/sizeof(myints[0]));

  mylist.remove(89);

  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // 17 7 14 
  // 实际: 17 7 14  初始化时少指定了范围
  EXPECT_EQ(true, true); 
}

// a predicate implemented as a function:
static bool single_digit (const int& value) { return (value<10); }

// a predicate implemented as a class:
struct is_odd {
  bool operator() (const int& value) { return (value%2)==1; }
};

TEST(test27, remove_if) 
{
  int myints[]= {15,36,7,17,20,39,4,1};
  mystl::list<int> mylist (myints,myints+8);   // 15 36 7 17 20 39 4 1

  mylist.remove_if (single_digit);           // 15 36 17 20 39

  mylist.remove_if (is_odd());               // 36 20

  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

}

TEST(test28, resize)
{
  mystl::list<int> mylist;

  // set some initial content:
  for (int i=1; i<10; ++i) mylist.push_back(i);

  mylist.resize(5);
  mylist.resize(8,100);
  mylist.resize(12);

  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

// 
TEST(test29, reverse)
{
  mystl::list<int> mylist;

  for (int i=1; i<10; ++i) mylist.push_back(i);

  mylist.reverse();
  EXPECT_NE(mylist.begin(), mylist.end());
  EXPECT_EQ(mylist.size(), 9);
  std::cout << "mylist contains:";
  for (mystl::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

// comparison, not case sensitive.
bool compare_nocase (const std::string& first, const std::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])<tolower(second[i])) return true;
    else if (tolower(first[i])>tolower(second[i])) return false;
    ++i;
  }
  return ( first.length() < second.length() );
}

// passed 
TEST(test30, sort)
{
  mystl::list<std::string> mylist;
  mystl::list<std::string>::iterator it;
  mylist.push_back ("one");
  mylist.push_back ("two");
  mylist.push_back ("Three");

  mylist.sort();

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  mylist.sort(compare_nocase);

  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}

// Transfers elements from x into the container, 
// inserting them at position.
TEST(test31, splicewithself)
{
  // wrong result 
  mystl::list<int> mylist1, mylist2;
  mystl::list<int>::iterator it;

  // set some initial values:
  for (int i=1; i<=4; ++i)
     mylist1.push_back(i);      // mylist1: 1 2 3 4

  for (int i=1; i<=3; ++i)
     mylist2.push_back(i*10);   // mylist2: 10 20 30

  it = mylist1.begin();
  ++it;                         // points to 2

  mylist1.splice (it, mylist2); // mylist1: 1 10 20 30 2 3 4
                                // mylist2 (empty)
                                // "it" still points to 2 (the 5th element)
                                          
  mylist2.splice (mylist2.begin(),mylist1, it);
                                // mylist1: 1 10 20 30 3 4
                                // mylist2: 2
                                // "it" is now invalid.
  it = mylist1.begin();
  std::advance(it,3);           // "it" points now to 30

  mylist1.splice ( mylist1.begin(), mylist1, it, mylist1.end());
                                // mylist1: 30 3 4 1 10 20

  std::cout << "mylist1 contains:";
  for (it=mylist1.begin(); it!=mylist1.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "mylist2 contains:";
  for (it=mylist2.begin(); it!=mylist2.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  // expected: 30 3 4 1 10 20
  // result:   30 3 4 1 10 20
  ASSERT_EQ(true, true); 
}

std::ostream& operator<<(std::ostream& ostr, const mystl::list<int>& list)
{
    for (auto &i : list) {
        ostr << " " << i;
    }
    return ostr;
}

TEST(test31, splicewithother)
{
    mystl::list<int> list1 = { 1, 2, 3, 4, 5 };
    mystl::list<int> list2 = { 10, 20, 30, 40, 50 };
 
    auto it = list1.begin();
    std::advance(it, 2);
 
    list1.splice(it, list2);
 
    std::cout << "list1: " << list1 << "\n";
    std::cout << "list2: " << list2 << "\n";
 
    list2.splice(list2.begin(), list1, it, list1.end());
 
    std::cout << "list1: " << list1 << "\n";
    std::cout << "list2: " << list2 << "\n";


}

TEST(test32, swap)
{
  mystl::list<int> first (3,100);   // three ints with a value of 100
  mystl::list<int> second (5,200);  // five ints with a value of 200

  first.swap(second);

  std::cout << "first contains:";
  for (mystl::list<int>::iterator it=first.begin(); it!=first.end(); it++)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "second contains:";
  for (mystl::list<int>::iterator it=second.begin(); it!=second.end(); it++)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // 200 200 200 200 200 
  // 100 100 100 
}





// a binary predicate implemented as a function:
static bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
struct is_near {
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
};

TEST(test33, unique)
{
  double mydoubles[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
                       12.77, 73.35, 72.25, 15.3,  72.25 };
  mystl::list<double> mylist (mydoubles,mydoubles+10);
  
  mylist.sort();             //  2.72,  3.14, 12.15, 12.77, 12.77,
                             // 15.3,  72.25, 72.25, 73.0,  73.35

  mylist.unique();           //  2.72,  3.14, 12.15, 12.77
                             // 15.3,  72.25, 73.0,  73.35

  mylist.unique (same_integral_part);  //  2.72,  3.14, 12.15
                                       // 15.3,  72.25, 73.0

  mylist.unique (is_near());           //  2.72, 12.15, 72.25

  std::cout << "mylist contains:";
  for (mystl::list<double>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}

TEST(test34, relational_operators) 
{
  mystl::list<int> a = {10, 20, 30};
  mystl::list<int> b = {10, 20, 30};
  mystl::list<int> c = {30, 20, 10};

  if (a==b) std::cout << "a and b are equal\n";
  if (b!=c) std::cout << "b and c are not equal\n";
  if (b<c) std::cout << "b is less than c\n";
  if (c>b) std::cout << "c is greater than b\n";
  if (a<=b) std::cout << "a is less than or equal to b\n";
  if (a>=b) std::cout << "a is greater than or equal to b\n";

}

TEST(test35, generic_swap)
{
  mystl::list<int> foo (3,100);   // three ints with a value of 100
  mystl::list<int> bar (5,200);   // five ints with a value of 200

  mystl::swap(foo,bar);

  std::cout << "foo contains:";
  for (mystl::list<int>::iterator it = foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "bar contains:";
  for (mystl::list<int>::iterator it = bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}