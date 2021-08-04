#include "queue.h" 
#include "deque.h" 
#include "list.h"
#include <string>  
#include <gtest/gtest.h> 


TEST(test1, ctor) 
{
    mystl::deque<int> mydeck (3,100);        // deque with 3 elements
    mystl::list<int> mylist (2,200);         // list with 2 elements

    mystl::queue<int> first;                 // empty queue
    mystl::queue<int> second (mydeck);       // queue initialized to copy of deque

    mystl::queue<int,mystl::list<int> > third; // empty queue with list as underlying container
    mystl::queue<int,mystl::list<int> > fourth (mylist);

    std::cout << "size of first: " << first.size() << '\n';
    std::cout << "size of second: " << second.size() << '\n';
    std::cout << "size of third: " << third.size() << '\n';
    std::cout << "size of fourth: " << fourth.size() << '\n';

}

TEST(test2, back)
{
    mystl::queue<int> myqueue;

    myqueue.push(12);
    myqueue.push(75);   // this is now the back

    myqueue.back() -= myqueue.front();

    std::cout << "myqueue.back() is now " << myqueue.back() << '\n';

}

TEST(test3, emplace)
{
    mystl::queue<std::string> myqueue;

    myqueue.emplace ("First sentence");
    myqueue.emplace ("Second sentence");

    std::cout << "myqueue contains:\n";
    while (!myqueue.empty())
    {
        std::cout << myqueue.front() << '\n';
        myqueue.pop();
    }
}

TEST(test4, empty)
{
    mystl::queue<std::string> myqueue;

    myqueue.emplace ("First sentence");
    myqueue.emplace ("Second sentence");
    myqueue.emplace ("Third sentence");

    std::cout << "myqueue contains:\n";
    while (!myqueue.empty())
    {
        std::cout << myqueue.front() << '\n';
        myqueue.pop();
    }
}

TEST(test5, front)
{
    mystl::queue<int> myqueue;

    myqueue.push(77);
    myqueue.push(16);

    myqueue.front() -= myqueue.back();    // 77-16=61

    std::cout << "myqueue.front() is now " << myqueue.front() << '\n'; 
}

TEST(test6, pop)
{
    mystl::queue<int> myqueue;

    for(int i = 0; i < 10; ++i)  
    {
        myqueue.push(i);
    }

    std::cout << "myqueue contains: ";
    while (!myqueue.empty())
    {
        std::cout << ' ' << myqueue.front();
        myqueue.pop();
    }
    std::cout << '\n';
}

TEST(test7, push)
{
    mystl::queue<int> myqueue;

    for(int i = 9; i >=0; --i)  
    {
        myqueue.push(i);
    }

    std::cout << "myqueue contains: ";
    while (!myqueue.empty())
    {
        std::cout << ' ' << myqueue.front();
        myqueue.pop();
    }
    std::cout << '\n';
}

TEST(test8, size)
{
    mystl::queue<int> myints;
    std::cout << "0. size: " << myints.size() << '\n';

    for (int i=0; i<5; i++) myints.push(i);
    std::cout << "1. size: " << myints.size() << '\n';

    myints.pop();
    std::cout << "2. size: " << myints.size() << '\n';
}

TEST(test9, swap)
{
    mystl::queue<int> foo,bar;
    foo.push (10); foo.push(20); foo.push(30);
    bar.push (111); bar.push(222);

    foo.swap(bar);

    std::cout << "size of foo: " << foo.size() << '\n';
    std::cout << "size of bar: " << bar.size() << '\n';
}

TEST(test10, generic_swap)
{
    mystl::queue<int> foo,bar;
    foo.push (10); foo.push(20); foo.push(30);
    bar.push (111); bar.push(222);


    mystl::swap(foo,bar);

    std::cout << "size of foo: " << foo.size() << '\n';
  std::cout << "size of bar: " << bar.size() << '\n';
}