#include <iostream> 
#include <gtest/gtest.h>
#include <string>
#include "memory.h" 

// test code for  cppreference.com  
# if 1
TEST(test1, uninitialized_copy) 
{
  std::string numbers[] = {"one","two","three"};

  // get block of uninitialized memory:
  std::pair <std::string*,std::ptrdiff_t> result = mystl::get_temporary_buffer<std::string>(3);

  if (result.second>0) {
    mystl::uninitialized_copy ( numbers, numbers+result.second, result.first );

    for (int i=0; i<result.second; i++)
      std::cout << result.first[i] << " ";
    std::cout << '\n';
   
    std::return_temporary_buffer(result.first);
  }


  int arr[5]{1,2,3,4,5}; 
    std::pair <int*,std::ptrdiff_t> result1 = mystl::get_temporary_buffer<int>(5);

  if (result1.second>0) {
    mystl::uninitialized_copy ( arr, arr+result1.second, result1.first );

    for (int i=0; i<result1.second; i++)
      std::cout << result1.first[i] << " ";
    std::cout << '\n';

    // deallocate buffer 
    std::return_temporary_buffer(result1.first);
  }


}
#endif 

TEST(test2, is_trivially_copy_assignable)
{
    using namespace std; 
    cout << is_trivially_copy_assignable<string>::value << endl;
    cout << is_trivially_copy_assignable<int>::value << endl; 
    cout << is_trivially_destructible<string>::value << endl;  
    EXPECT_EQ((is_trivially_copy_assignable<string>::value), false); 
    EXPECT_EQ((is_trivially_copy_assignable<int>::value), true);
}

TEST(test3, pairtest)
{
    mystl::pair<std::string*, int> p1(nullptr, 0); 
    mystl::pair<std::string*, int> p2 = p1; 

}

TEST(test4, uninitialized_copy_n)
{
  std::string numbers[] = {"one","two","three"};

  // get block of uninitialized memory:
  std::pair <std::string*,std::ptrdiff_t> result = std::get_temporary_buffer<std::string>(3);

  if (result.second>0) 
  {
    mystl::uninitialized_copy_n ( numbers, result.second, result.first );

    for (int i=0; i<result.second; i++)
      std::cout << result.first[i] << ' ';
    std::cout << '\n';

    std::return_temporary_buffer(result.first);
  }

    int arr[5]{1,2,3,4,5};
    std::pair<int*, std::ptrdiff_t> result1 = std::get_temporary_buffer<int>(5); 

    if(result1.second >0 )
    {
        mystl::uninitialized_copy_n(arr, arr + 5, result1.first); 
        for (int i=0; i<result1.second; i++)
            std::cout << result1.first[i] << ' ';
        std::cout << '\n';
    }
    std::return_temporary_buffer(result1.first);
}

TEST(test5, uninitialized_fill)
{
  // get block of uninitialized memory:
  std::pair <std::string*,std::ptrdiff_t>
    result = std::get_temporary_buffer<std::string>(3);

  if (result.second>0) {
    mystl::uninitialized_fill ( result.first, result.first+result.second, "c++ rocks!" );

    for (int i=0; i<result.second; i++)
      std::cout << result.first[i] << '\n';

    std::return_temporary_buffer(result.first);
  }

    int arr[5]{1,2,3,4,5}; 
    std::pair <int*, ptrdiff_t> res1 
        = std::get_temporary_buffer<int>(5); 

    if(res1.second > 0)
    {
        mystl::uninitialized_fill(res1.first, res1.first + res1.second, 5);
        for (int i=0; i<res1.second; i++)
            std::cout << res1.first[i] << '\n';
        std::return_temporary_buffer(res1.first);
    }

}

TEST(test6, uninitialized_fill_n)
{

    std::pair <std::string*,std::ptrdiff_t>
    result = std::get_temporary_buffer<std::string>(3);

  if (result.second>0) {
    mystl::uninitialized_fill_n ( result.first, result.second, "c++ rocks!" );

    for (int i=0; i<result.second; i++)
    {
        std::cout << result.first[i] << '\n';
        EXPECT_EQ(result.first[i], "c++ rocks!");
    }
      

    std::return_temporary_buffer(result.first);
  }

    int arr[5]{1,2,3,4,5}; 
    std::pair <int*, ptrdiff_t> res1 
        = std::get_temporary_buffer<int>(5); 

    if(res1.second > 0)
    {
        mystl::uninitialized_fill_n(res1.first, res1.second, 5);
        for (int i=0; i<res1.second; i++)
        {
            EXPECT_EQ(res1.first[i], 5);
            std::cout<<res1.first[i] <<" ";
        }
        std::return_temporary_buffer(res1.first);
    }

}

// std::move() 
TEST(test7, uninitialized_move)
{
  std::vector<std::string> foo = {"air","water","fire","earth"};
  std::vector<std::string> bar (4);

  // moving ranges:
  std::cout << "Moving ranges...\n";
  mystl::uninitialized_move (foo.begin(), foo.begin()+4, bar.begin() );

  std::cout << "foo contains " << foo.size() << " elements:";
  std::cout << " (each in an unspecified but valid state)";
  std::cout << '\n';

  std::cout << "bar contains " << bar.size() << " elements:";
  for (std::string& x: bar) std::cout << " [" << x << "]";
  std::cout << '\n';

  // moving container:
  std::cout << "Moving container...\n";
  foo = std::move(bar);

  std::cout << "foo contains " << foo.size() << " elements:";
  for (std::string& x: foo) std::cout << " [" << x << "]";
  std::cout << '\n';

  std::cout << "bar is in an unspecified but valid state";
  std::cout << '\n';

}

TEST(test8, uninitialized_move_n) 
{
    std::vector<std::string> foo = {"air","water","fire","earth"};
  std::vector<std::string> bar (4);

  // moving ranges:
  std::cout << "Moving ranges...\n";
  mystl::uninitialized_move_n(foo.begin(), 4, bar.begin() );

  std::cout << "foo contains " << foo.size() << " elements:";
  std::cout << " (each in an unspecified but valid state)";
  std::cout << '\n';

  std::cout << "bar contains " << bar.size() << " elements:";
  for (std::string& x: bar) std::cout << " [" << x << "]";
  std::cout << '\n';

  // moving container:
  std::cout << "Moving container...\n";
  foo = mystl::move(bar);

  std::cout << "foo contains " << foo.size() << " elements:";
  for (std::string& x: foo) std::cout << " [" << x << "]";
  std::cout << '\n';

  std::cout << "bar is in an unspecified but valid state";
  std::cout << '\n';
}


// 类型萃取的结果都是编译期常量，可以试着使用typeid().name() 来查看信息
TEST(test9, iterator_traits)
{
    using std::cout; 
    using std::endl; 
 

    cout << typeid(mystl::iterator_traits<
    std::vector<int>::iterator>::value_type).name()<< endl; 
    cout << typeid(mystl::iterator_traits<
    std::vector<int>::iterator>::reference).name()<< endl;
    cout << typeid(mystl::iterator_traits<
    std::vector<int>::iterator>::pointer).name()<< endl;
    cout << typeid(mystl::iterator_traits<
    std::vector<int>::iterator>::iterator_category).name()<< endl;
    cout << typeid(std::random_access_iterator_tag).name() << endl; 
    cout << typeid(mystl::iterator_traits<
    std::vector<int>::iterator>::difference_type).name()<< endl;

    const char* nameStr1 =  typeid(std::random_access_iterator_tag).name(); 
    const char* nameStr2 = typeid(mystl::iterator_traits<std::vector<int>::iterator>::iterator_category).name(); 
    EXPECT_EQ (nameStr1, nameStr2); 
}

// 想要使用mystl::iterator_triats 萃取 std::container<T>::iterator 类型
// 必须要在实现mystl::iteartor_traits 时继承 std::xxx_iterator_tag 否则会导致迭代器类型萃取失败
TEST(test10, isTagSame)
{
    using std::cout; 
    using std::endl; 
    using std::string;
    string nameStr1 = typeid(std::input_iterator_tag).name(); 
    string nameStr2 = typeid(mystl::input_iterator_tag).name(); 

    EXPECT_NE(nameStr1, nameStr2); 
    cout << nameStr1 <<endl; 
    cout << nameStr2 << endl; 

}
