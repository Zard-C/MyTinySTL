#include <gtest/gtest.h> 
#include "stack.h" 
#include <vector>
#include <string>  



TEST(test1, ctor)
{
    mystl::deque<int> mydeque (3,100);          // deque with 3 elements
    std::vector<int> myvector (2,200);        // vector with 2 elements

    mystl::stack<int> first;                    // empty stack
    mystl::stack<int> second (mydeque);         // stack initialized to copy of deque

    mystl::stack<int,std::vector<int> > third;  // empty stack using vector
    mystl::stack<int,std::vector<int> > fourth (myvector);

    std::cout << "size of first: " << first.size() << '\n';
    std::cout << "size of second: " << second.size() << '\n';
    std::cout << "size of third: " << third.size() << '\n';
    std::cout << "size of fourth: " << fourth.size() << '\n';
}

TEST(test2, emplace) 
{
    mystl::stack<std::string> mystack;

    mystack.emplace ("First sentence");
    mystack.emplace ("Second sentence");

    std::cout << "mystack contains:\n";
    while (!mystack.empty())
    {
    std::cout << mystack.top() << '\n';
    mystack.pop();
    }
}

TEST(test3, empty)
{
    mystl::stack<int> mystack;
    int sum (0);

    for (int i=1;i<=10;i++) mystack.push(i);

    while (!mystack.empty())
    {
        sum += mystack.top();
        mystack.pop();
    }

    std::cout << "total: " << sum << '\n';
}

TEST(test4, pop)
{
    mystl::stack<int> mystack;

    for (int i=0; i<5; ++i) mystack.push(i);

    std::cout << "Popping out elements...";
    while (!mystack.empty())
    {
        std::cout << ' ' << mystack.top();
        mystack.pop();
    }
    std::cout << '\n';
}

TEST(test5, push)
{
    mystl::stack<int> mystack;

    for (int i=0; i<5; ++i) mystack.push(i);

    std::cout << "Popping out elements...";
    while (!mystack.empty())
    {
        std::cout << ' ' << mystack.top();
        mystack.pop();
    }
    std::cout << '\n';
}

TEST(test6, size)
{
  mystl::stack<int> myints;
  std::cout << "0. size: " << myints.size() << '\n';

  for (int i=0; i<5; i++) myints.push(i);
  std::cout << "1. size: " << myints.size() << '\n';

  myints.pop();
  std::cout << "2. size: " << myints.size() << '\n';

}

TEST(test7, swap)
{
    mystl::stack<int> foo,bar;
    foo.push (10); foo.push(20); foo.push(30);
    bar.push (111); bar.push(222);

    foo.swap(bar);

    std::cout << "size of foo: " << foo.size() << '\n';
    std::cout << "size of bar: " << bar.size() << '\n';

}