#include <gtest/gtest.h> 
#include "map.h"

static bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp 
{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

TEST(test1, ctors)
{
  mystl::map<char,int > first;
  
  std::cout << sizeof(mystl::pair<char, int>) << std::endl; 
  std::cout << sizeof(mystl::rb_tree<mystl::pair<char,int>, mystl::less<char>>::base_type)<< std::endl;
  #if 1
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
  
#endif 

}