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


// clear then push_back -> suppressed errors 
TEST(test8, clear)
{
    unsigned int i;
    mystl::deque<int> mydeque;
    mydeque.clear();
    mydeque.push_back (100);
    mydeque.push_back (200);
    mydeque.push_back (300);

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    
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



// emplace_back: segment fault 
TEST(test10, emplace)
{
    mystl::deque<int> mydeque = {10,20,30};

    auto it = mydeque.emplace ( mydeque.begin()+1, 100 );
    mydeque.emplace ( it, 200 );
    mydeque.emplace ( mydeque.end(), 300 );

    std::cout << "mydeque contains:";
    for (auto& x: mydeque)
    std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test11, emplace_back)
{
    mystl::deque<int> mydeque = {10,20,30};

    mydeque.emplace_back (100);
    mydeque.emplace_back (200);

    std::cout << "mydeque contains:";
    for (auto& x: mydeque)
        std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test12, emplace_front)
{
    mystl::deque<int> mydeque = {10,20,30};

    mydeque.emplace_front (111);
    mydeque.emplace_front (222);

    std::cout << "mydeque contains:";
    for (auto& x: mydeque)
        std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test13, empty)
{
    mystl::deque<int> mydeque;
    int sum (0);

    for (int i=1;i<=10;i++) mydeque.push_back(i);

    while (!mydeque.empty())
    {
        sum += mydeque.front();
        mydeque.pop_front();
    }

    std::cout << "total: " << sum << '\n';
}

TEST(test14, end)
{
    mystl::deque<int> mydeque;

    for (int i=1; i<=5; i++) mydeque.insert(mydeque.end(),i);

    std::cout << "mydeque contains:";

    mystl::deque<int>::iterator it = mydeque.begin();

    while (it != mydeque.end() )
    std::cout << ' ' << *it++;

    std::cout << '\n';
}

TEST(test15, erase)
{
    mystl::deque<int> mydeque;

    // set some values (from 1 to 10)
    for (int i=1; i<=10; i++) mydeque.push_back(i);

    // erase the 6th element
    mydeque.erase (mydeque.begin()+5);

    // erase the first 3 elements:
    mydeque.erase (mydeque.begin(),mydeque.begin()+3);

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test16, front)
{
    mystl::deque<int> mydeque;

    mydeque.push_front(77);
    mydeque.push_back(20);

    mydeque.front() -= mydeque.back();

    std::cout << "mydeque.front() is now " << mydeque.front() << '\n';
}

TEST(test17, get_allocator) 
{
    mystl::deque<int> mydeque;
    int * p;
    unsigned int i;

    // allocate an array with space for 5 elements using deque's allocator:
    p = mydeque.get_allocator().allocate(5);

    // construct values in-place on the array:
    for (i=0; i<5; i++) mydeque.get_allocator().construct(&p[i],i);

    std::cout << "The allocated array contains:";
    for (i=0; i<5; i++) std::cout << ' ' << p[i];
    std::cout << '\n';

    // destroy and deallocate:
    for (i=0; i<5; i++) mydeque.get_allocator().destroy(&p[i]);
    mydeque.get_allocator().deallocate(p,5);
}

TEST(test18, insert)
{
    mystl::deque<int> mydeque;

    // set some initial values:
    for (int i=1; i<6; i++) mydeque.push_back(i); // 1 2 3 4 5

    mystl::deque<int>::iterator it = mydeque.begin();
    ++it;

    it = mydeque.insert (it,10);                  // 1 10 2 3 4 5
    // "it" now points to the newly inserted 10

    mydeque.insert (it,2,20);                     // 1 20 20 10 2 3 4 5
    // "it" no longer valid!

    it = mydeque.begin()+2;

    std::vector<int> myvector (2,30);
    mydeque.insert (it,myvector.begin(),myvector.end());
                                                // 1 20 30 30 20 10 2 3 4 5

    std::cout << "mydeque contains:";
    for (it=mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

}


// memory-leaking 
TEST(test19, operator_equal)
{
    mystl::deque<int> first (3);    // deque with 3 zero-initialized ints
    mystl::deque<int> second (5);   // deque with 5 zero-initialized ints

    second = first;                 // copy assign 
    first = mystl::deque<int>();    // move assign 

    std::cout << "Size of first: " << int (first.size()) << '\n';
    std::cout << "Size of second: " << int (second.size()) << '\n';
}

TEST(test20, operator_access)
{
    mystl::deque<int> mydeque (10);   // 10 zero-initialized elements
    mystl::deque<int>::size_type sz = mydeque.size();

    // assign some values:
    for (unsigned i=0; i<sz; i++) mydeque[i]=i;

    // reverse order of elements using operator[]:
    for (unsigned i=0; i<sz/2; i++)
    {
    int temp;
    temp = mydeque[sz-1-i];
    mydeque[sz-1-i]=mydeque[i];
    mydeque[i]=temp;
    }

    // print content:
    std::cout << "mydeque contains:";
    for (unsigned i=0; i<sz; i++)
    std::cout << ' ' << mydeque[i];
    std::cout << '\n';
}

TEST(test21, pop_back)
{
    mystl::deque<int> mydeque;
    int sum (0);
    mydeque.push_back (10);
    mydeque.push_back (20);
    mydeque.push_back (30);

    while (!mydeque.empty())
    {
        sum+=mydeque.back();
        mydeque.pop_back();
    }

    std::cout << "The elements of mydeque add up to " << sum << '\n';
}

TEST(test22, pop_front) 
{
    mystl::deque<int> mydeque;

  mydeque.push_back (100);
  mydeque.push_back (200);
  mydeque.push_back (300);

  std::cout << "Popping out the elements in mydeque:";
  while (!mydeque.empty())
  {
    std::cout << ' ' << mydeque.front();
    mydeque.pop_front();
  }

  std::cout << "\nThe final size of mydeque is " << int(mydeque.size()) << '\n';
}

TEST(test23, push_back)
{
    mystl::deque<int> mydeque; 
    for(int i = 0; i < 1024; ++i)  
    {
        mydeque.push_back(i); 
    }
    std::cout << "mydeque stores " << (int) mydeque.size() << " numbers.\n";
}

TEST(test24, rbegin_rend)
{
    mystl::deque<int> mydeque (5);  // 5 default-constructed ints

    mystl::deque<int>::reverse_iterator rit = mydeque.rbegin();

    int i=0;
    for (rit = mydeque.rbegin(); rit!= mydeque.rend(); ++rit)
    *rit = ++i;

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it != mydeque.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test25, resize)
{
    mystl::deque<int> mydeque;
    mystl::deque<int>::iterator it;

    // set some initial content:
    for (int i=1; i<10; ++i) mydeque.push_back(i);

    mydeque.resize(5);
    mydeque.resize(8,100);
    mydeque.resize(12);

    std::cout << "mydeque contains:";
    for (mystl::deque<int>::iterator it = mydeque.begin(); it != mydeque.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}

TEST(test26, shrink_to_fit)
{
    mystl::deque<double> mydeque (100);
    std::cout << "1. size of mydeque: " << mydeque.size() << '\n';

    int a = 10; 
    auto it = mydeque.end(); 
    std::cout << it.cur<<" ,";
    std::cout << it.last << "\n"; 
    mydeque.shrink_to_fit();

    

    it = mydeque.end(); 
    std::cout << it.cur<<"\n";
    mydeque.push_back(10);

    std::cout << mydeque.size() <<", " << mydeque.back() << '\n';
}

TEST(test27, swap)
{
    unsigned int i;
    mystl::deque<int> foo (3,100);   // three ints with a value of 100
    mystl::deque<int> bar (5,200);   // five ints with a value of 200

    foo.swap(bar);

    std::cout << "foo contains:";
    for (mystl::deque<int>::iterator it = foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

    std::cout << "bar contains:";
    for (mystl::deque<int>::iterator it = bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';

}

TEST(test28, relational_opeartor)
{
    mystl::deque<int> foo (3,100);   // three ints with a value of 100
  mystl::deque<int> bar (2,200);   // two ints with a value of 200

  if (foo==bar) std::cout << "foo and bar are equal\n";
  if (foo!=bar) std::cout << "foo and bar are not equal\n";
  if (foo< bar) std::cout << "foo is less than bar\n";
  if (foo> bar) std::cout << "foo is greater than bar\n";
  if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
  if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";

}

TEST(test29, generic_swap)
{
    unsigned int i;
  mystl::deque<int> foo (3,100);   // three ints with a value of 100
  mystl::deque<int> bar (5,200);   // five ints with a value of 200

  swap(foo,bar);

  std::cout << "foo contains:";
  for (mystl::deque<int>::iterator it = foo.begin(); it!=foo.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "bar contains:";
  for (mystl::deque<int>::iterator it = bar.begin(); it!=bar.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

}