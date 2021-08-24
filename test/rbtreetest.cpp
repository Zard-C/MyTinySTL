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
  mystl::map<char,int > first;
  
  std::cout << sizeof(mystl::pair<char, int>) << std::endl; 
  std::cout << sizeof(mystl::rb_tree<mystl::pair<char,int>, mystl::less<char>>::base_type)<< std::endl;

  first['a']=10;
  first['b']=30;
  first['c']=50;
  first['d']=70;

  mystl::map<char,int> second (first.begin(),first.end());

  mystl::map<char,int> third (second);

  mystl::map<char,int,classcomp> fourth;                 // class as Compare

  bool(*fn_pt)(char,char) = fncomp;
  std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compar
  // encountered memory leak, leak size = 32, a size of rb_tree_node_base<T>

}

TEST(test2, at)
{
  mystl::map<std::string,int> mymap = {
                { "alpha", 0 },
                { "beta", 0 },
                { "gamma", 0 } };

  mymap.at("alpha") = 10;
  mymap.at("beta") = 20;
  mymap.at("gamma") = 30;

  for (auto& x: mymap) {
    std::cout << x.first << ": " << x.second << '\n';
  }
}

TEST(test3, begin)
{
  mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}

TEST(test4, cbegin)
{
   mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // print content:
  std::cout << "mymap contains:";
  for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
    std::cout << " [" << (*it).first << ':' << (*it).second << ']';
  std::cout << '\n';
}

TEST(test5, cend)
{
  mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // print content:
  std::cout << "mymap contains:";
  for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
    std::cout << " [" << (*it).first << ':' << (*it).second << ']';
  std::cout << '\n';
}

TEST(test6, clear)
{

  mystl::map<char,int> mymap;

  mymap['x']=100;
  mymap['y']=200;
  mymap['z']=300;

  std::cout << "mymap contains:\n";
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';


  mymap.clear();
  std::cout << mymap.size() << std::endl;
  mymap['a']=1101;
  mymap['b']=2202;
  
  std::cout << "mymap contains:\n";
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
  std::cout << mymap.size() << std::endl;
  // abort ->  pointer being freed was not allocated
}   


TEST(test7, count) 
{
    mystl::map<char,int> mymap;
  char c;

  mymap ['a']=101;
  mymap ['c']=202;
  mymap ['f']=303;

  for (c='a'; c<'h'; c++)
  {
    std::cout << c;
    if (mymap.count(c)>0)
      std::cout << " is an element of mymap.\n";
    else 
      std::cout << " is not an element of mymap.\n";
  }
}

TEST(test8, crbegin)
{
  mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  std::cout << "mymap backwards:";
  for (auto rit = mymap.crbegin(); rit != mymap.crend(); ++rit)
    std::cout << " [" << rit->first << ':' << rit->second << ']';
  std::cout << '\n';
}

TEST(test9, crend)
{
  mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  std::cout << "mymap backwards:";
  for (auto rit = mymap.crbegin(); rit != mymap.crend(); ++rit)
    std::cout << " [" << rit->first << ':' << rit->second << ']';
  std::cout << '\n';
}

TEST(test10, emplace)
{
  mystl::map<char,int> mymap;

  mymap.emplace('x',100);
  mymap.emplace('y',200);
  mymap.emplace('z',100);

  std::cout << "mymap contains:";
  for (auto& x: mymap)
    std::cout << " [" << x.first << ':' << x.second << ']';
  std::cout << '\n';
}

TEST(test11, emplace_hint)
{
   mystl::map<char,int> mymap;
  auto it = mymap.end();

  it = mymap.emplace_hint(it,'b',10);
  mymap.emplace_hint(it,'a',12);
  mymap.emplace_hint(mymap.end(),'c',14);

  std::cout << "mymap contains:";
  for (auto& x: mymap)
    std::cout << " [" << x.first << ':' << x.second << ']';
  std::cout << '\n';
}

TEST(test12, empty)
{
  mystl::map<char,int> mymap;

  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;

  while (!mymap.empty())
  {
    std::cout << mymap.begin()->first << " => " << mymap.begin()->second << '\n';
    mymap.erase(mymap.begin());
  }


}

TEST(test13, end)
{
  mystl::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

}

TEST(test14, equal_range)
{
  mystl::map<char,int> mymap;

  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;

  mystl::pair<mystl::map<char,int>::iterator, mystl::map<char,int>::iterator> ret;
  ret = mymap.equal_range('b');

  std::cout << "lower bound points to: ";
  std::cout << ret.first->first << " => " << ret.first->second << '\n';

  std::cout << "upper bound points to: ";
  std::cout << ret.second->first << " => " << ret.second->second << '\n';
}

TEST(test15, erase)
{
  mystl::map<char,int> mymap;
  mystl::map<char,int>::iterator it;

  // insert some values:
  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;
  mymap['d']=40;
  mymap['e']=50;
  mymap['f']=60;

  it=mymap.find('b');
  mymap.erase (it);                   // erasing by iterator

  mymap.erase ('c');                  // erasing by key

  it=mymap.find ('e');
  mymap.erase ( it, mymap.end() );    // erasing by range

  // show content:
  for (it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}

TEST(test16, find)
{

  mystl::map<char,int> mymap;
  mystl::map<char,int>::iterator it;

  mymap['a']=50;
  mymap['b']=100;
  mymap['c']=150;
  mymap['d']=200;

  it = mymap.find('b');
  if (it != mymap.end())
    mymap.erase (it);

  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap.find('a')->second << '\n';
  std::cout << "c => " << mymap.find('c')->second << '\n';
  std::cout << "d => " << mymap.find('d')->second << '\n';
}

TEST(test17, get_allocator)
{ 
  int psize;
  std::map<char,int> mymap;
  std::pair<const char,int>* p;

  // allocate an array of 5 elements using mymap's allocator:
  p=mymap.get_allocator().allocate(5);

  // assign some values to array
  psize = sizeof(std::map<char,int>::value_type)*5;

  std::cout << "The allocated array has a size of " << psize << " bytes.\n";

  mymap.get_allocator().deallocate(p,5);

}

TEST(test18, insert)
{
  mystl::map<char,int> mymap;

  // first insert function version (single parameter):
  mymap.insert ( mystl::pair<const char,int>('a',100) );
  mymap.insert ( mystl::pair<const char,int>('z',200) );

  mystl::pair<mystl::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( mystl::pair<const char,int>('z',500) );
  if (ret.second==false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

  // second insert function version (with hint position):
  mystl::map<const char,int>::iterator it = mymap.begin();
  mymap.insert (it, mystl::pair<const char,int>('b',300));  // max efficiency inserting
  mymap.insert (it, mystl::pair<const char,int>('c',400));  // no max efficiency inserting

  // third insert function version (range insertion):
  mystl::map<const char,int> anothermap;
  anothermap.insert(mymap.begin(),mymap.find('c'));

  // showing contents:
  std::cout << "mymap contains:\n";
  for (it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "anothermap contains:\n";
  for (it=anothermap.begin(); it!=anothermap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}

TEST(test19, key_comp)
{
  mystl::map<char,int> mymap;

  mystl::map<char,int>::key_compare mycomp = mymap.key_comp();

  mymap['a']=100;
  mymap['b']=200;
  mymap['c']=300;

  std::cout << "mymap contains:\n";

  char highest = mymap.rbegin()->first;     // key value of last element

  mystl::map<char,int>::iterator it = mymap.begin();
  do {
    std::cout << it->first << " => " << it->second << '\n';
  } while ( mycomp((*it++).first, highest) );

  std::cout << '\n';
}

TEST(test20, lower_bound) 
{
  mystl::map<char,int> mymap;
  mystl::map<char,int>::iterator itlow,itup;

  mymap['a']=20;
  mymap['b']=40;
  mymap['c']=60;
  mymap['d']=80;
  mymap['e']=100;

  itlow=mymap.lower_bound ('b');  // itlow points to b
  itup=mymap.upper_bound ('d');   // itup points to e (not d!)

  mymap.erase(itlow,itup);        // erases [itlow,itup)

  // print content:
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';


}

TEST(test21, max_size)
{
  int i;
  mystl::map<int,int> mymap;

  if (mymap.max_size()>1000)
  {
    for (i=0; i<1000; i++) mymap[i]=0;
    std::cout << "The map contains 1000 elements.\n";
  }
  else std::cout << "The map could not hold 1000 elements.\n";

}

TEST(test22, operator_equal)
{
  mystl::map<char,int> first;
  mystl::map<char,int> second;

  first['x']=8;
  first['y']=16;
  first['z']=32;

  second=first;                // second now contains 3 ints
  first=mystl::map<char,int>();  // and first is now empty

  std::cout << "Size of first: " << first.size() << '\n';
  std::cout << "Size of second: " << second.size() << '\n';
  // memory-leak 32 bytes in opeartor=(rb_tree && rhs)
}

TEST(test23, operator_reference)
{
  mystl::map<char,std::string> mymap;

  mymap['a']="an element";
  mymap['b']="another element";
  mymap['c']=mymap['b'];

  std::cout << "mymap['a'] is " << mymap['a'] << '\n';
  std::cout << "mymap['b'] is " << mymap['b'] << '\n';
  std::cout << "mymap['c'] is " << mymap['c'] << '\n';
  std::cout << "mymap['d'] is " << mymap['d'] << '\n';

  std::cout << "mymap now contains " << mymap.size() << " elements.\n";
}

TEST(test24, rbegin)
{
  mystl::map<char,int> mymap;

  mymap['x'] = 100;
  mymap['y'] = 200;
  mymap['z'] = 300;

  // show content:
  mystl::map<char,int>::reverse_iterator rit;
  for (rit=mymap.rbegin(); rit!=mymap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';

}

TEST(test25, rend)
{
  mystl::map<char,int> mymap;

  mymap['x'] = 100;
  mymap['y'] = 200;
  mymap['z'] = 300;

  // show content:
  mystl::map<char,int>::reverse_iterator rit;
  for (rit=mymap.rbegin(); rit!=mymap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';

}

TEST(test26, size)
{
  mystl::map<char,int> mymap;
  mymap['a']=101;
  mymap['b']=202;
  mymap['c']=302;

  std::cout << "mymap.size() is " << mymap.size() << '\n';

}

TEST(test27, swap)
{
  mystl::map<char,int> foo,bar;

  foo['x']=100;
  foo['y']=200;

  bar['a']=11;
  bar['b']=22;
  bar['c']=33;

  foo.swap(bar);

  std::cout << "foo contains:\n";
  for (mystl::map<char,int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "bar contains:\n";
  for (mystl::map<char,int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}

TEST(test28, upper_bound)
{
  mystl::map<char,int> mymap;
  mystl::map<char,int>::iterator itlow,itup;

  mymap['a']=20;
  mymap['b']=40;
  mymap['c']=60;
  mymap['d']=80;
  mymap['e']=100;

  itlow=mymap.lower_bound ('b');  // itlow points to b
  itup=mymap.upper_bound ('d');   // itup points to e (not d!)

  mymap.erase(itlow,itup);        // erases [itlow,itup)

  // print content:
  for (mystl::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}

TEST(test29, value_comp)
{
  mystl::map<char,int> mymap;

  mymap['x']=1001;
  mymap['y']=2002;
  mymap['z']=3003;

  std::cout << "mymap contains:\n";

  //mystl::pair<char,int> highest = *mymap.rbegin();          // last element
  // *mymap.begin() is a pair<const char, int> type 
  std::cout << std::is_same<mystl::pair<const char, int>, mystl::map<char, int>::value_type>::value << std::endl;
}

// remove_cv 
TEST(test30, remove_cv)
{
    typedef std::remove_cv<const int>::type type1;
    typedef std::remove_cv<volatile int>::type type2;
    typedef std::remove_cv<const volatile int>::type type3;
    typedef std::remove_cv<const volatile int*>::type type4;
    typedef std::remove_cv<int * const volatile>::type type5;
 
    std::cout << "test1 " << (std::is_same<int, type1>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test2 " << (std::is_same<int, type2>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test3 " << (std::is_same<int, type3>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test4 " << (std::is_same<const volatile int*, type4>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test5 " << (std::is_same<int*, type5>::value
        ? "passed" : "failed") << '\n';
}

#if 1
TEST(test31, insert)
{
  mystl::map<char,int> mymap;

  // first insert function version (single parameter):
  mymap.insert ( mystl::pair<char,int>('a',100) );
  mymap.insert ( mystl::pair<char,int>('z',200) );

  mystl::pair<mystl::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( mystl::pair<char,int>('z',500) );
  if (ret.second==false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

}
#endif 

TEST(test32, pair)
{
  std::pair<char, int>(); 
  mystl::pair<char, int> p1 = {'a', 1}; 
  
  mystl::pair<const char, int> p2 = p1;
  typedef const char Other1;
  typedef int        Other2; 
  typedef const char U1; 
  typedef int        U2; 

  typedef char        T1; 
  typedef int          T2;

  std::cout << 
    (std::is_default_constructible<Other1>::value &&
    std::is_default_constructible<Other2>::value) << std::endl;

    std::cout << 
      (std::is_copy_constructible<U1>::value && 
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value && 
    std::is_convertible<const U2&, T2>::value) << std::endl;

    std::cout << (std::is_constructible<T1, Other1>::value && 
     std::is_constructible<T2, Other2>::value && 
     (!std::is_convertible<Other1, T1>::value || 
      !std::is_convertible<Other2, T1>::value)) << std::endl;
  
}