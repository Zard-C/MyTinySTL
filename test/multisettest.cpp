// multisettest.cpp -- test codes for container adaptor multiset 
#include <gtest/gtest.h>  
#include "set.h" 

static bool fncomp (int lhs, int rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const int& lhs, const int& rhs) const
  {return lhs<rhs;}
};

TEST(test1, ctors_and_dtor)
{
    mystl::multiset<int> first;                          // empty multiset of ints

    int myints[]= {10,20,30,20,20};
    mystl::multiset<int> second (myints,myints+5);       // pointers used as iterators

    mystl::multiset<int> third (second);                 // a copy of second

    mystl::multiset<int> fourth (second.begin(), second.end());  // iterator ctor.

    mystl::multiset<int,classcomp> fifth;                // class as Compare

}

TEST(test2, begin)
{
    int myints[] = {42,71,71,71,12};
    mystl::multiset<int> mymultiset (myints,myints+5);

    mystl::multiset<int>::iterator it;

    std::cout << "mymultiset contains:";
    for (mystl::multiset<int>::iterator it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test3, cbegin)
{
    mystl::multiset<int> mymultiset = {10,20,30,20,10};

    std::cout << "mymultiset contains:";
    for (auto it=mymultiset.cbegin(); it != mymultiset.cend(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';

}

TEST(test4, cend)
{
    mystl::multiset<int> mymultiset = {10,20,30,20,10};

    std::cout << "mymultiset contains:";
    for (auto it=mymultiset.cbegin(); it != mymultiset.cend(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test5, clear)
{
    mystl::multiset<int> mymultiset;

    mymultiset.insert (11);
    mymultiset.insert (42);
    mymultiset.insert (11);

    std::cout << "mymultiset contains:";
    for (mystl::multiset<int>::iterator it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    mymultiset.clear();
    mymultiset.insert (200);
    mymultiset.insert (100);

    std::cout << "mymultiset contains:";
    for (mystl::multiset<int>::iterator it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test6, count)
{
    int myints[]={10,73,12,22,73,73,12};
    mystl::multiset<int> mymultiset (myints,myints+7);

    std::cout << "73 appears " << mymultiset.count(73) << " times in mymultiset.\n";

}

TEST(test7, crbegin)
{
    mystl::multiset<int> mymultiset = {10,20,30,20,10};

    std::cout << "mymultiset backwards:";
    for (auto rit=mymultiset.crbegin(); rit != mymultiset.crend(); ++rit)
        std::cout << ' ' << *rit;

    std::cout << '\n';
}

TEST(test8, crend)
{
    mystl::multiset<int> mymultiset = {10,20,30,20,10};

    std::cout << "mymultiset backwards:";
    for (auto rit=mymultiset.crbegin(); rit != mymultiset.crend(); ++rit)
    std::cout << ' ' << *rit;

    std::cout << '\n';
}

TEST(test9, emplace) 
{
    mystl::multiset<std::string> mymultiset;

    mymultiset.emplace("foo");
    mymultiset.emplace("bar");
    mymultiset.emplace("foo");

    std::cout << "mymultiset contains:";
    for (const std::string& x: mymultiset)
        std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test10, emplace_hint)
{
    mystl::multiset<std::string> mymultiset;
    auto it = mymultiset.cbegin();

    mymultiset.emplace_hint (it,"apple");
    it = mymultiset.emplace_hint (mymultiset.cend(),"orange");
    it = mymultiset.emplace_hint (it,"melon");
    mymultiset.emplace_hint (it,"melon");

    std::cout << "mymultiset contains:";
    for (const std::string& x: mymultiset)
        std::cout << ' ' << x;
    std::cout << '\n';

}

TEST(test11, empty)
{
    mystl::multiset<int> mymultiset;

    mymultiset.insert(10);
    mymultiset.insert(20);
    mymultiset.insert(10);

    std::cout << "mymultiset contains:";
    while (!mymultiset.empty())
    {
        std::cout << ' ' << *mymultiset.begin();
        mymultiset.erase(mymultiset.begin());
    }
    std::cout << '\n';
}

TEST(test12, end) 
{
    int myints[] = {15,98,77,77,39};
    mystl::multiset<int> mymultiset (myints,myints+5);

    std::cout << "mymultiset contains:";
    for (mystl::multiset<int>::iterator it=mymultiset.begin(); it!=mymultiset.end(); ++it )
        std::cout << ' ' << *it;

    std::cout << '\n';
}

typedef mystl::multiset<int>::iterator It;  // aliasing the iterator type used
TEST(test13, equal_range) 
{
    int myints[]= {77,30,16,2,30,30};
    mystl::multiset<int> mymultiset (myints, myints+6);  // 2 16 30 30 30 77

    mystl::pair<It,It> ret = mymultiset.equal_range(30); //      ^        ^

    mymultiset.erase(ret.first,ret.second);

    std::cout << "mymultiset contains:";
    for (It it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

}

TEST(test14, erase)
{
    mystl::multiset<int> mymultiset;
    mystl::multiset<int>::iterator it;

    // insert some values:
    mymultiset.insert (40);                            // 40
    for (int i=1; i<7; i++) mymultiset.insert(i*10);   // 10 20 30 40 40 50 60

    it=mymultiset.begin();
    it++;                                              //    ^

    mymultiset.erase (it);                             // 10 30 40 40 50 60

    mymultiset.erase (40);                             // 10 30 50 60

    it=mymultiset.find (50);
    mymultiset.erase ( it, mymultiset.end() );         // 10 30

    std::cout << "mymultiset contains:";
    for (it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test15, find) 
{
    mystl::multiset<int> mymultiset;
    mystl::multiset<int>::iterator it;

    // set some initial values:
    for (int i=1; i<=5; i++) mymultiset.insert(i*10);   // 10 20 30 40 50

    it=mymultiset.find(20);
    mymultiset.erase (it);
    mymultiset.erase (mymultiset.find(40));

    std::cout << "mymultiset contains:";
    for (it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test16, get_allocator)
{
    mystl::multiset<int> mymultiset;
    int * p;
    unsigned int i;

    // allocate an array of 5 elements using myset's allocator:
    p=mymultiset.get_allocator().allocate(5);

    // assign some values to array
    for (i=0; i<5; i++) p[i]=(i+1)*10;

    std::cout << "The allocated array contains:";
    for (i=0; i<5; i++) std::cout << ' ' << p[i];
    std::cout << '\n';

    mymultiset.get_allocator().deallocate(p,5);
}

TEST(test17, insert)
{
    mystl::multiset<int> mymultiset;
    mystl::multiset<int>::iterator it;

    // set some initial values:
    for (int i=1; i<=5; i++) mymultiset.insert(i*10);  // 10 20 30 40 50

    it=mymultiset.insert(25);

    it=mymultiset.insert (it,27);    // max efficiency inserting
    it=mymultiset.insert (it,29);    // max efficiency inserting
    it=mymultiset.insert (it,24);    // no max efficiency inserting (24<29)

    int myints[]= {5,10,15};
    mymultiset.insert (myints,myints+3);

    std::cout << "mymultiset contains:";
    for (it=mymultiset.begin(); it!=mymultiset.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test18, key_comp)
{
    mystl::multiset<int> mymultiset;

    for (int i=0; i<5; i++) mymultiset.insert(i);

    mystl::multiset<int>::key_compare mycomp = mymultiset.key_comp();

    std::cout << "mymultiset contains:";

    int highest = *mymultiset.rbegin();

    mystl::multiset<int>::iterator it = mymultiset.begin();
    do 
    {
        std::cout << ' ' << *it;
    } while ( mycomp(*it++,highest) );

    std::cout << '\n';
}