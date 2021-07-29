#include <gtest/gtest.h> 
#include "deque.h" 
#include <iostream>  
#include <string>  



TEST(test1, ctors)
{

    unsigned int i;

    // constructors used in the same order as described above:
    mystl::deque<int> first;                                // empty deque of ints
    mystl::deque<int> second (4,100);                       // four ints with value 100
    mystl::deque<int> third (second.begin(),second.end());  // iterating through second
    mystl::deque<int> fourth (third);                       // a copy of third

    // the iterator constructor can be used to copy arrays:
    int myints[] = {16,2,77,29};
    mystl::deque<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

    std::cout << "deque memory: "; 
    std::cout << fifth.begin().node <<",  " ;
    std::cout << fifth.begin().cur << ", "
    << fifth.begin().first <<", " << fifth.begin().last << "\n";    

    std::cout << "The contents of fifth are:";
    for (mystl::deque<int>::iterator it = fifth.begin(); it!=fifth.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

// clear function 
TEST(test2, dtor)
{
    std::string arr[4] = {"one", "two", "three", "four"}; 
    mystl::deque<std::string> c(arr, arr + sizeof(arr)/sizeof(arr[0])); 

    std::cout<<"The contents of deque<string> are:"; 
    for(auto it = c.begin(); it != c.end(); ++it)
    {
        std::cout << ' ' << *it; 
    }
    std::cout<<'\n'; 
}


// memory leak ?!  clear 函数中没有将buffer释放
TEST(test3, assign) 
{
    mystl::deque<int> first;
    mystl::deque<int> second;
    mystl::deque<int> third;

    first.assign (7,100);             // 7 ints with a value of 100

    mystl::deque<int>::iterator it;
    it=first.begin()+1;

    second.assign (it,first.end()-1); // the 5 central values of first

    int myints[] = {1776,7,4};
    third.assign (myints,myints+3);   // assigning from array.

    std::cout << "Size of first: " << int (first.size()) << '\n';
    std::cout << "Size of second: " << int (second.size()) << '\n';
    std::cout << "Size of third: " << int (third.size()) << '\n';

    for(auto it = first.begin(); it != first.end(); ++it)
    {
        std::cout << " " <<*it ; 
    }
    std::cout << "\n"; 
}

TEST(test4, at)
{
    mystl::deque<unsigned> mydeque (10);   // 10 zero-initialized unsigneds

  // assign some values:
  for (unsigned i=0; i<mydeque.size(); i++)
    mydeque.at(i)=i;

  std::cout << "mydeque contains:";
  for (unsigned i=0; i<mydeque.size(); i++)
    std::cout << ' ' << mydeque.at(i);

  std::cout << '\n';
}

TEST(test5, back)
{
    mystl::deque<int> mydeque;

  mydeque.push_back(10);

  while (mydeque.back() != 0)
    mydeque.push_back ( mydeque.back() -1 );

  std::cout << "mydeque contains:";

  for (mystl::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
}

TEST(test6, begin)
{
    mystl::deque<int> mydeque;

    for (int i=1; i<=5; i++) mydeque.push_back(i);

    std::cout << "mydeque contains:";

    mystl::deque<int>::iterator it = mydeque.begin();

    while (it != mydeque.end())
    std::cout << ' ' << *it++;

    std::cout << '\n';
}

TEST(test7, cbegin_cend)
{
    mystl::deque<int> mydeque = {10,20,30,40,50};

    std::cout << "mydeque contains:";

    for (auto it = mydeque.cbegin(); it != mydeque.cend(); ++it)
    std::cout << ' ' << *it;

    std::cout << '\n';
}

TEST(test8, clear)
{
    unsigned int i;
    mystl::deque<int> mydeque;
    mydeque.push_back (100);
    mydeque.push_back (200);
    mydeque.push_back (300);

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    mydeque.clear();
    mydeque.push_back (1101);
    mydeque.push_back (2202);

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test9, crbegin_crend)
{
    mystl::deque<int> mydeque = {1,2,3,4,5};

    std::cout << "mydeque backwards:";
    for (auto rit = mydeque.crbegin(); rit != mydeque.crend(); ++rit)
    std::cout << ' ' << *rit;
    std::cout << '\n';
}