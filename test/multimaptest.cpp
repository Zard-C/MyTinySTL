#include <gtest/gtest.h>
#include <string> 
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


TEST(test5, clear)
{
   mystl::multimap<char,int> mymultimap;
  mystl::multimap<char,int>::iterator it;

  mymultimap.insert(mystl::pair<char,int>('b',80));
  mymultimap.insert(mystl::pair<char,int>('b',120));
  mymultimap.insert(mystl::pair<char,int>('q',360));

  std::cout << "mymultimap contains:\n";
  for (it=mymultimap.begin(); it!=mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

  mymultimap.clear();

  mymultimap.insert(mystl::pair<char,int>('a',11));
  mymultimap.insert(mystl::pair<char,int>('x',22));

  std::cout << "mymultimap contains:\n";
  for (it=mymultimap.begin(); it != mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

}


TEST(test6, count )
{
mystl::multimap<char,int> mymm;

  mymm.insert(mystl::make_pair('x',50));
  mymm.insert(mystl::make_pair('y',100));
  mymm.insert(mystl::make_pair('y',150));
  mymm.insert(mystl::make_pair('y',200));
  mymm.insert(mystl::make_pair('z',250));
  mymm.insert(mystl::make_pair('z',300));

  for (char c='x'; c<='z'; c++)
  {
    std::cout << "There are " << mymm.count(c) << " elements with key " << c << ":";
    mystl::multimap<char,int>::iterator it;
    for (it=mymm.equal_range(c).first; it!=mymm.equal_range(c).second; ++it)
      std::cout << ' ' << (*it).second;
    std::cout << '\n';
  }

}

TEST(test7, crbegin)
{
  mystl::multimap<char,int> mymultimap = { {'x',100}, {'y',200}, {'x',300} };

  // print content:
  std::cout << "mymultimap backwards:";
  for (auto rit = mymultimap.crbegin(); rit != mymultimap.crend(); ++rit)
    std::cout << " [" << rit->first << ':' << rit->second << ']';
  std::cout << '\n';

}

TEST(test8, crend)
{
  mystl::multimap<char,int> mymultimap = { {'x',100}, {'y',200}, {'x',300} };

  // print content:
  std::cout << "mymultimap backwards:";
  for (auto rit = mymultimap.crbegin(); rit != mymultimap.crend(); ++rit)
    std::cout << " [" << rit->first << ':' << rit->second << ']';
  std::cout << '\n';
}

TEST(test9, emplace )
{
   mystl::multimap<std::string,float> mymultimap;

  mymultimap.emplace("apple",1.50);
  mymultimap.emplace("coffee",2.10);
  mymultimap.emplace("apple",1.40);

  std::cout << "mymultimap contains:";
  for (auto& x: mymultimap)
    std::cout << " [" << x.first << ':' << x.second << ']';
  std::cout << '\n';
}

TEST(test10, emplace_hint)  
{
  mystl::multimap<std::string,int> mymultimap;
  auto it = mymultimap.end();

  it = mymultimap.emplace_hint(it,"foo",10);
  mymultimap.emplace_hint(it,"bar",20);
  mymultimap.emplace_hint(mymultimap.end(),"foo",30);

  std::cout << "mymultimap contains:";
  for (auto& x: mymultimap)
    std::cout << " [" << x.first << ':' << x.second << ']';
  std::cout << '\n';
}

TEST(test11, empty)
{
  mystl::multimap<char,int> mymultimap;

  mymultimap.insert (mystl::pair<char,int>('b',101));
  mymultimap.insert (mystl::pair<char,int>('b',202));
  mymultimap.insert (mystl::pair<char,int>('q',505));

  while (!mymultimap.empty())
  {
     std::cout << mymultimap.begin()->first << " => ";
     std::cout << mymultimap.begin()->second << '\n';
     mymultimap.erase(mymultimap.begin());
  }
}

TEST(test12, end)
{
  mystl::multimap<char,int> mymultimap;

  mymultimap.insert (mystl::pair<char,int>('a',10));
  mymultimap.insert (mystl::pair<char,int>('b',20));
  mymultimap.insert (mystl::pair<char,int>('b',150));

  // show content:
  for (mystl::multimap<char,int>::iterator it=mymultimap.begin(); it!=mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';
}

TEST(test13, equal_range)
{
  mystl::multimap<char,int> mymm;

  mymm.insert(mystl::pair<char,int>('a',10));
  mymm.insert(mystl::pair<char,int>('b',20));
  mymm.insert(mystl::pair<char,int>('b',30));
  mymm.insert(mystl::pair<char,int>('b',40));
  mymm.insert(mystl::pair<char,int>('c',50));
  mymm.insert(mystl::pair<char,int>('c',60));
  mymm.insert(mystl::pair<char,int>('d',60));

  std::cout << "mymm contains:\n";
  for (char ch='a'; ch<='d'; ch++)
  {
    mystl::pair <mystl::multimap<char,int>::iterator, mystl::multimap<char,int>::iterator> ret;
    ret = mymm.equal_range(ch);
    std::cout << ch << " =>";
    for (mystl::multimap<char,int>::iterator it=ret.first; it!=ret.second; ++it)
      std::cout << ' ' << it->second;
    std::cout << '\n';
  }

}


TEST(test14, erase)
{
  mystl::multimap<char,int> mymultimap;

  // insert some values:
  mymultimap.insert(mystl::pair<char,int>('a',10));
  mymultimap.insert(mystl::pair<char,int>('b',20));
  mymultimap.insert(mystl::pair<char,int>('b',30));
  mymultimap.insert(mystl::pair<char,int>('c',40));
  mymultimap.insert(mystl::pair<char,int>('d',50));
  mymultimap.insert(mystl::pair<char,int>('d',60));
  mymultimap.insert(mystl::pair<char,int>('e',70));
  mymultimap.insert(mystl::pair<char,int>('f',80));


  mystl::multimap<char,int>::iterator it = mymultimap.find('b');


  mymultimap.erase (it);                     // erasing by iterator (1 element)

  mymultimap.erase ('d');                    // erasing by key (2 elements)

  it=mymultimap.find ('e');
  mymultimap.erase ( it, mymultimap.end() ); // erasing by range

  // show content:
  for (it=mymultimap.begin(); it!=mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';


// abort: pointer being freed was not allocated
}

TEST(test15, find)
{
  mystl::multimap<char,int> mymm;

  mymm.insert (mystl::make_pair('x',10));
  mymm.insert (mystl::make_pair('y',20));
  mymm.insert (mystl::make_pair('z',30));
  mymm.insert (mystl::make_pair('z',40));

  mystl::multimap<char,int>::iterator it = mymm.find('x');
  mymm.erase (it);
  mymm.erase (mymm.find('z'));

  // print content:
  std::cout << "elements in mymm:" << '\n';
  std::cout << "y => " << mymm.find('y')->second << '\n';
  std::cout << "z => " << mymm.find('z')->second << '\n';

}

TEST(test16, get_allocator)
{
   int psize;
  mystl::multimap<char,int> mymm;
  mystl::pair<const char,int>* p;

  // allocate an array of 5 elements using mymm's allocator:
  p=mymm.get_allocator().allocate(5);

  // assign some values to array
  psize = sizeof(mystl::multimap<char,int>::value_type)*5;

  std::cout << "The allocated array has a size of " << psize << " bytes.\n";

  mymm.get_allocator().deallocate(p,5);
}

TEST(test17, insert)
{
  mystl::multimap<char,int> mymultimap;
  mystl::multimap<char,int>::iterator it;

  // first insert function version (single parameter):
  mymultimap.insert ( mystl::pair<char,int>('a',100) );
  mymultimap.insert ( mystl::pair<char,int>('z',150) );
  it=mymultimap.insert ( mystl::pair<char,int>('b',75) );

  // second insert function version (with hint position):
  mymultimap.insert (it, mystl::pair<char,int>('c',300));  // max efficiency inserting
  mymultimap.insert (it, mystl::pair<char,int>('z',400));  // no max efficiency inserting

  // third insert function version (range insertion):
  mystl::multimap<char,int> anothermultimap;
  anothermultimap.insert(mymultimap.begin(),mymultimap.find('c'));

  // showing contents:
  std::cout << "mymultimap contains:\n";
  for (it=mymultimap.begin(); it!=mymultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

  std::cout << "anothermultimap contains:\n";
  for (it=anothermultimap.begin(); it!=anothermultimap.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';
}

TEST(test18, key_compare)
{
  mystl::multimap<char,int> mymultimap;

  mystl::multimap<char,int>::key_compare mycomp = mymultimap.key_comp();

  mymultimap.insert (mystl::make_pair('a',100));
  mymultimap.insert (mystl::make_pair('b',200));
  mymultimap.insert (mystl::make_pair('b',211));
  mymultimap.insert (mystl::make_pair('c',300));

  std::cout << "mymultimap contains:\n";

  char highest = mymultimap.rbegin()->first;     // key value of last element

  mystl::multimap<char,int>::iterator it = mymultimap.begin();
  do {
    std::cout << (*it).first << " => " << (*it).second << '\n';
  } while ( mycomp((*it++).first, highest) );

  std::cout << '\n';

}


TEST(test19, lower_bound)
{
  mystl::multimap<char,int> mymultimap;
  mystl::multimap<char,int>::iterator it,itlow,itup;

  mymultimap.insert(mystl::make_pair('a',10));
  mymultimap.insert(mystl::make_pair('b',121));
  mymultimap.insert(mystl::make_pair('c',1001));
  mymultimap.insert(mystl::make_pair('c',2002));
  mymultimap.insert(mystl::make_pair('d',11011));
  mymultimap.insert(mystl::make_pair('e',44));

  itlow = mymultimap.lower_bound ('b');  // itlow points to b
  itup = mymultimap.upper_bound ('d');   // itup points to e (not d)

  // print range [itlow,itup):
  for (it=itlow; it!=itup; ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

}

TEST(test20, max_size)
{

  mystl::multimap<int,int> mymultimap;

  if (mymultimap.max_size() >= 1000u)
  {
    for (int i=0; i<1000; i++)
      mymultimap.insert(mystl::make_pair(i,0));
    std::cout << "The multimap contains 1000 elements.\n";
  }
  else std::cout << "The multimap could not hold 1000 elements.\n";


}

TEST(test21, operator_equal)
{
  mystl::multimap<char,int> foo,bar;

  foo.insert(mystl::make_pair('x',32));
  foo.insert(mystl::make_pair('y',64));
  foo.insert(mystl::make_pair('y',96));
  foo.insert(mystl::make_pair('z',128));

  bar = foo;                  // bar now contains 4 ints
  foo.clear();                // and first is now empty

  std::cout << "Size of foo: " << foo.size() << '\n';
  std::cout << "Size of bar: " << bar.size() << '\n';
}

TEST(test22, rbegin)
{
  mystl::multimap<char,int> mymultimap;

  mymultimap.insert (mystl::make_pair('x',10));
  mymultimap.insert (mystl::make_pair('y',20));
  mymultimap.insert (mystl::make_pair('y',150));
  mymultimap.insert (mystl::make_pair('z',9));

  // show content:
  mystl::multimap<char,int>::reverse_iterator rit;
  for (rit=mymultimap.rbegin(); rit!=mymultimap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';


}


TEST(test23, rend)
{
  mystl::multimap<char,int> mymultimap;

  mymultimap.insert (mystl::make_pair('x',10));
  mymultimap.insert (mystl::make_pair('y',20));
  mymultimap.insert (mystl::make_pair('y',150));
  mymultimap.insert (mystl::make_pair('z',9));

  // show content:
  mystl::multimap<char,int>::reverse_iterator rit;
  for (rit=mymultimap.rbegin(); rit!=mymultimap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';
}

TEST(test24, size)
{
   mystl::multimap<char,int> mymultimap;

  mymultimap.insert(mystl::make_pair('x',100));
  mymultimap.insert(mystl::make_pair('y',200));
  mymultimap.insert(mystl::make_pair('y',350));
  mymultimap.insert(mystl::make_pair('z',500));

  std::cout << "mymultimap.size() is " << mymultimap.size() << '\n';
}

TEST(test25, swap)
{
  mystl::multimap<char,int> foo,bar;

  foo.insert(mystl::make_pair('x',100));
  foo.insert(mystl::make_pair('y',200));

  bar.insert(mystl::make_pair('a',11));
  bar.insert(mystl::make_pair('b',22));
  bar.insert(mystl::make_pair('a',55));

  foo.swap(bar);

  std::cout << "foo contains:\n";
  for (mystl::multimap<char,int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

  std::cout << "bar contains:\n";
  for (mystl::multimap<char,int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

}

TEST(test26, upper_bound)
{
  mystl::multimap<char,int> mymultimap;
  mystl::multimap<char,int>::iterator it,itlow,itup;

  mymultimap.insert(mystl::make_pair('a',10));
  mymultimap.insert(mystl::make_pair('b',121));
  mymultimap.insert(mystl::make_pair('c',1001));
  mymultimap.insert(mystl::make_pair('c',2002));
  mymultimap.insert(mystl::make_pair('d',11011));
  mymultimap.insert(mystl::make_pair('e',44));

  itlow = mymultimap.lower_bound ('b');  // itlow points to b
  itup = mymultimap.upper_bound ('d');   // itup points to e (not d)

  // print range [itlow,itup):
  for (it=itlow; it!=itup; ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';
}

TEST(test27, value_comp)
{
  mystl::multimap<char,int> mymultimap;

  mymultimap.insert(mystl::make_pair('x',101));
  mymultimap.insert(mystl::make_pair('y',202));
  mymultimap.insert(mystl::make_pair('y',252));
  mymultimap.insert(mystl::make_pair('z',303));

  std::cout << "mymultimap contains:\n";

  mystl::pair<char,int> highest = *mymultimap.rbegin();          // last element

  mystl::multimap<char,int>::iterator it = mymultimap.begin();
  do {
    std::cout << (*it).first << " => " << (*it).second << '\n';
  } while ( mymultimap.value_comp()(*it++, highest) );

}

TEST(test28, relational_operator)
{
  mystl::multimap<char,int> foo,bar;

  foo.insert (mystl::make_pair('a',100));
  foo.insert (mystl::make_pair('z',900));
  bar.insert (mystl::make_pair('b',250));
  bar.insert (mystl::make_pair('b',350));

  // foo ({{a,100},{z,900}}) vs bar ({b,250},{b,350}}):
  if (foo==bar) std::cout << "foo and bar are equal\n";
  if (foo!=bar) std::cout << "foo and bar are not equal\n";
  if (foo< bar) std::cout << "foo is less than bar\n";
  if (foo> bar) std::cout << "foo is greater than bar\n";
  if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
  if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
}

TEST(test29, generic_swap)
{
  mystl::multimap<char,int> foo,bar;

  foo.insert(mystl::make_pair('x',100));
  foo.insert(mystl::make_pair('y',200));

  bar.insert(mystl::make_pair('a',11));
  bar.insert(mystl::make_pair('b',22));
  bar.insert(mystl::make_pair('a',55));

  swap(foo,bar);

  std::cout << "foo contains:\n";
  for (mystl::multimap<char,int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

  std::cout << "bar contains:\n";
  for (mystl::multimap<char,int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';

}