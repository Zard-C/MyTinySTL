#include <gtest/gtest.h>
#include "map.h"



static bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp 
{
    bool operator() (const char& lhs, const char& rhs) const
    {return lhs<rhs;}
};




TEST(test1, ctors_and_dtors)
{
    mystl::multimap<char,int> first;

  first.insert(mystl::pair<char,int>('a',10));
  first.insert(mystl::pair<char,int>('b',15));
  first.insert(mystl::pair<char,int>('b',20));
  first.insert(mystl::pair<char,int>('c',25));

  mystl::multimap<char,int> second (first.begin(),first.end());

  mystl::multimap<char,int> third (second);

  mystl::multimap<char,int,classcomp> fourth;                 // class as Compare

  bool(*fn_pt)(char,char) = fncomp;
  std::multimap<char,int, bool(*)(char,char)> fifth (fn_pt); // function pointer as comp
}

TEST(test2, begin)
{
    mystl::multimap<char,int> mymultimap;

  mymultimap.insert (mystl::pair<char,int>('a',10));
  mymultimap.insert (mystl::pair<char,int>('b',20));
  mymultimap.insert (mystl::pair<char,int>('b',150));

  // show content:
  for (mystl::multimap<char,int>::iterator it=mymultimap.begin(); it!=mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';
}

TEST(test3, cbegin)
{
    mystl::multimap<char,int> mymultimap = { {'x',100}, {'y',200}, {'x',300} };

  // print content:
  std::cout << "mymultimap contains:";
  for (auto it = mymultimap.cbegin(); it != mymultimap.cend(); ++it)
    std::cout << " [" << it->first << ':' << it->second << ']';
  std::cout << '\n';
}

TEST(test4, cend)
{
     mystl::multimap<char,int> mymultimap = { {'x',100}, {'y',200}, {'x',300} };

  // print content:
  std::cout << "mymultimap contains:";
  for (auto it = mymultimap.cbegin(); it != mymultimap.cend(); ++it)
    std::cout << " [" << it->first << ':' << it->second << ']';
  std::cout << '\n';
}

