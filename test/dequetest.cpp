#include <gtest/gtest.h> 
#include "deque.h" 
#include <iostream>  


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

