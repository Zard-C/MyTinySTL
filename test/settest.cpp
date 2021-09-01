// settest.cpp -- test code for container adapter set 
#include <gtest/gtest.h> 
#include "set.h"

bool fncomp (int lhs, int rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const int& lhs, const int& rhs) const
  {return lhs<rhs;}
};


TEST(test1, ctor_and_dtor)
{
    mystl::set<int> first;                           // empty set of ints

  int myints[]= {10,20,30,40,50};
  mystl::set<int> second (myints,myints+5);        // range

  mystl::set<int> third (second);                  // a copy of second

  mystl::set<int> fourth (second.begin(), second.end());  // iterator ctor.

  mystl::set<int,classcomp> fifth;                 // class as Compare

}

TEST(test2, begin)
{
    int myints[] = {75,23,65,42,13};
  mystl::set<int> myset (myints,myints+5);

  std::cout << "myset contains:";
  for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

TEST(test3, cbegin)
{
    mystl::set<int> myset = {50,20,60,10,25};

    std::cout << "myset contains:";
    for (auto it=myset.cbegin(); it != myset.cend(); ++it)
        std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test4, cend)
{
    mystl::set<int> myset = {50,20,60,10,25};

    std::cout << "myset contains:";
        for (auto it=myset.cbegin(); it != myset.cend(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test5, clear)
{
    mystl::set<int> myset;

  myset.insert (100);
  myset.insert (200);
  myset.insert (300);

  std::cout << "myset contains:";
  for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  myset.clear();
  myset.insert (1101);
  myset.insert (2202);

  std::cout << "myset contains:";
  for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
}

TEST(test6, count)
{
    mystl::set<int> myset;

    // set some initial values:
    for (int i=1; i<5; ++i) myset.insert(i*3);    // set: 3 6 9 12

    for (int i=0; i<10; ++i)
    {
        std::cout << i;
    if (myset.count(i)!=0)
        std::cout << " is an element of myset.\n";
    else
        std::cout << " is not an element of myset.\n";
    }
}

TEST(test7, crbegin)
{
    mystl::set<int> myset = {50,20,60,10,25};

    std::cout << "myset backwards:";
        for (auto rit=myset.crbegin(); rit != myset.crend(); ++rit)
    std::cout << ' ' << *rit;

    std::cout << '\n';
}

TEST(test8, crend)
{
    mystl::set<int> myset = {50,20,60,10,25};

    std::cout << "myset backwards:";
    for (auto rit=myset.crbegin(); rit != myset.crend(); ++rit)
        std::cout << ' ' << *rit;

    std::cout << '\n';
}

TEST(test8, emplace)
{
    mystl::set<std::string> myset;
    myset.emplace("foo");
    myset.emplace("bar");
    auto ret = myset.emplace("foo");

    if (!ret.second) std::cout << "foo already exists in myset\n";

    for (auto rit=myset.crbegin(); rit != myset.crend(); ++rit)
        std::cout << ' ' << *rit;
    std::cout << '\n';
}

TEST(test9, emplace_hint)
{
    mystl::set<std::string> myset;
    auto it = myset.cbegin();

    myset.emplace_hint (it,"alpha");
    it = myset.emplace_hint (myset.cend(),"omega");
    it = myset.emplace_hint (it,"epsilon");
    it = myset.emplace_hint (it,"beta");

    std::cout << "myset contains:";
    for (const std::string& x: myset)
        std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test10, empty)
{
    mystl::set<int> myset;

    myset.insert(20);
    myset.insert(30);
    myset.insert(10);

    std::cout << "myset contains:";
    while (!myset.empty())
    {
        std::cout << ' ' << *myset.begin();
        myset.erase(myset.begin());
    }
    std::cout << '\n';
}

TEST(test11, end)
{
    int myints[] = {75,23,65,42,13};
    mystl::set<int> myset (myints,myints+5);

    std::cout << "myset contains:";
    for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
        std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test12, equal_range)
{
    mystl::set<int> myset;

  for (int i=1; i<=5; i++) myset.insert(i*10);   // myset: 10 20 30 40 50

  mystl::pair<mystl::set<int>::const_iterator,mystl::set<int>::const_iterator> ret;
  ret = myset.equal_range(30);

  std::cout << "the lower bound points to: " << *ret.first << '\n';
  std::cout << "the upper bound points to: " << *ret.second << '\n';
}

TEST(test13, erase)
{
    mystl::set<int> myset;
    mystl::set<int>::iterator it;

    // insert some values:
    for (int i=1; i<10; i++) myset.insert(i*10);  // 10 20 30 40 50 60 70 80 90

    it = myset.begin();
    ++it;                                         // "it" points now to 20

    myset.erase (it);

    myset.erase (40);

    it = myset.find (60);
    myset.erase (it, myset.end());

    std::cout << "myset contains:";
    for (it=myset.begin(); it!=myset.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test14, find)
{
    mystl::set<int> myset;
    mystl::set<int>::iterator it;

    // set some initial values:
    for (int i=1; i<=5; i++) myset.insert(i*10);    // set: 10 20 30 40 50

    it=myset.find(20);
    myset.erase (it);
    myset.erase (myset.find(40));

    std::cout << "myset contains:";
    for (it=myset.begin(); it!=myset.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test15, get_allocator)
{
    mystl::set<int> myset;
    int * p;
    unsigned int i;

    // allocate an array of 5 elements using myset's allocator:
    p=myset.get_allocator().allocate(5);

    // assign some values to array
    for (i=0; i<5; i++) p[i]=(i+1)*10;

    std::cout << "The allocated array contains:";
    for (i=0; i<5; i++) std::cout << ' ' << p[i];
    std::cout << '\n';

    myset.get_allocator().deallocate(p,5);
}

TEST(test16, insert)
{
    mystl::set<int> myset;
  mystl::set<int>::iterator it;
  mystl::pair<mystl::set<int>::iterator,bool> ret;

  // set some initial values:
  for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

  ret = myset.insert(20);               // no new element inserted

  if (ret.second==false) it=ret.first;  // "it" now points to element 20

  myset.insert (it,25);                 // max efficiency inserting
  myset.insert (it,24);                 // max efficiency inserting
  myset.insert (it,26);                 // no max efficiency inserting

  int myints[]= {5,10,15};              // 10 already in set, not inserted
  myset.insert (myints,myints+3);

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

}

TEST(test17, key_comp)
{
    mystl::set<int> myset;
  int highest;

  mystl::set<int>::key_compare mycomp = myset.key_comp();

  for (int i=0; i<=5; i++) myset.insert(i);

  std::cout << "myset contains:";

  highest=*myset.rbegin();
  mystl::set<int>::iterator it=myset.begin();
  do {
    std::cout << ' ' << *it;
  } while ( mycomp(*(++it),highest) );

  std::cout << '\n';
}

TEST(test18, lower_bound)
{
    mystl::set<int> myset;
    mystl::set<int>::iterator itlow,itup;

    for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

    itlow=myset.lower_bound (30);                //       ^
    itup=myset.upper_bound (60);                 //                   ^

    myset.erase(itlow,itup);                     // 10 20 70 80 90

    std::cout << "myset contains:";
    for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test19, max_size)
{
    int i;
    mystl::set<int> myset;

    if (myset.max_size()>1000)
    {
        for (i=0; i<1000; i++) myset.insert(i);
        std::cout << "The set contains 1000 elements.\n";
    }
    else std::cout << "The set could not hold 1000 elements.\n";

}

TEST(test20, operator_equal)
{
    int myints[]={ 12,82,37,64,15 };
    mystl::set<int> first (myints,myints+5);   // set with 5 ints
    mystl::set<int> second;                    // empty set

    second = first;                          // now second contains the 5 ints
    first = mystl::set<int>();                 // and first is empty

    std::cout << "Size of first: " << int (first.size()) << '\n';
    std::cout << "Size of second: " << int (second.size()) << '\n';
}

TEST(test21, rbegin)
{
int myints[] = {21,64,17,78,49};
mystl::set<int> myset (myints,myints+5);

mystl::set<int>::reverse_iterator rit;

std::cout << "myset contains:";
for (rit=myset.rbegin(); rit != myset.rend(); ++rit)
    std::cout << ' ' << *rit;

std::cout << '\n';
}

TEST(test22, rend)
{
    int myints[] = {21,64,17,78,49};
    std::set<int> myset (myints,myints+5);

    std::set<int>::reverse_iterator rit;

    std::cout << "myset contains:";
    for (rit=myset.rbegin(); rit != myset.rend(); ++rit)
    std::cout << ' ' << *rit;

    std::cout << '\n';
}

TEST(test23, size)
{
    mystl::set<int> myints;
    std::cout << "0. size: " << myints.size() << '\n';

    for (int i=0; i<10; ++i) myints.insert(i);
    std::cout << "1. size: " << myints.size() << '\n';

    myints.insert (100);
    std::cout << "2. size: " << myints.size() << '\n';

    myints.erase(5);
    std::cout << "3. size: " << myints.size() << '\n';
}

TEST(test24, swap)
{
    int myints[]={12,75,10,32,20,25};
    mystl::set<int> first (myints,myints+3);     // 10,12,75
    mystl::set<int> second (myints+3,myints+6);  // 20,25,32

    first.swap(second);

    std::cout << "first contains:";
    for (mystl::set<int>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    std::cout << "second contains:";
    for (mystl::set<int>::iterator it=second.begin(); it!=second.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test25, upper_bound)
{
    mystl::set<int> myset;
    mystl::set<int>::iterator itlow,itup;

    for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

    itlow=myset.lower_bound (30);                //       ^
    itup=myset.upper_bound (60);                 //                   ^

    myset.erase(itlow,itup);                     // 10 20 70 80 90

    std::cout << "myset contains:";
    for (mystl::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test26, value_comp)
{
    mystl::set<int> myset;

    mystl::set<int>::value_compare mycomp = myset.value_comp();

    for (int i=0; i<=5; i++) myset.insert(i);

    std::cout << "myset contains:";

    int highest=*myset.rbegin();
    mystl::set<int>::iterator it=myset.begin();
    do {
    std::cout << ' ' << *it;
    } while ( mycomp(*(++it),highest) );

    std::cout << '\n';
}

TEST(test27, relational_operators)
{
    mystl::set<int> foo,bar;
  foo.insert(10);
  bar.insert(20);
  bar.insert(30);
  foo.insert(40);

  // foo ({10,40}) vs bar ({20,30}):
  if (foo==bar) std::cout << "foo and bar are equal\n";
  if (foo!=bar) std::cout << "foo and bar are not equal\n";
  if (foo< bar) std::cout << "foo is less than bar\n";
  if (foo> bar) std::cout << "foo is greater than bar\n";
  if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
  if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
}

TEST(test28, generic_swap)
{
    int myints[]={12,75,10,32,20,25};
    mystl::set<int> first (myints,myints+3);     // 10,12,75
    mystl::set<int> second (myints+3,myints+6);  // 20,25,32

    swap(first,second);

    std::cout << "first contains:";
    for (mystl::set<int>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    std::cout << "second contains:";
    for (mystl::set<int>::iterator it=second.begin(); it!=second.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}