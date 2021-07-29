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
