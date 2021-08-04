#include "queue.h" 
#include <gtest/gtest.h>  


class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (const int& lhs, const int&rhs) const
  {
    if (reverse) return (lhs>rhs);
    else return (lhs<rhs);
  }
};


TEST(test1, ctor)
{
    int myints[]= {10,60,50,20};

  mystl::priority_queue<int> first;
  mystl::priority_queue<int> second (myints,myints+4);
  mystl::priority_queue<int, std::vector<int>, std::greater<int> >
                            third (myints,myints+4);
  // using mycomparison:
  typedef mystl::priority_queue<int,std::vector<int>,mycomparison> mypq_type;

  mypq_type fourth;                       // less-than comparison
  mypq_type fifth (mycomparison(true));   // greater-than comparison

}

TEST(test2, emplace)
{
    mystl::priority_queue<std::string> mypq;

  mypq.emplace("orange");
  mypq.emplace("strawberry");
  mypq.emplace("apple");
  mypq.emplace("pear");

  std::cout << "mypq contains:";
  while (!mypq.empty())
  {
     std::cout << ' ' << mypq.top();
     mypq.pop();
  }
  std::cout << '\n';
}

TEST(test3, empty)
{
    mystl::priority_queue<int> mypq;
  int sum (0);

  for (int i=1;i<=10;i++) mypq.push(i);

  while (!mypq.empty())
  {
    std::cout << mypq.top() << " "; 
     sum += mypq.top();
     mypq.pop();
  }

  std::cout << "total: " << sum << '\n';
}

TEST(test4, pop)
{
    mystl::priority_queue<int> mypq;

  mypq.push(30);
  mypq.push(100);
  mypq.push(25);
  mypq.push(40);

  std::cout << "Popping out elements...";
  while (!mypq.empty())
  {
     std::cout << ' ' << mypq.top();
     mypq.pop();
  }
  std::cout << '\n';
}

TEST(test5, push)
{
     mystl::priority_queue<int> mypq;

  mypq.push(30);
  mypq.push(100);
  mypq.push(25);
  mypq.push(40);

  std::cout << "Popping out elements...";
  while (!mypq.empty())
  {
     std::cout << ' ' << mypq.top();
     mypq.pop();
  }
  std::cout << '\n';

}

TEST(test6, size)
{
     mystl::priority_queue<int> mypq;

  mypq.push(30);
  mypq.push(100);
  mypq.push(25);
  mypq.push(40);

  std::cout << "Popping out elements...";
  while (!mypq.empty())
  {
     std::cout << ' ' << mypq.top();
     mypq.pop();
  }
  std::cout << '\n';

}

TEST(test7, swap)
{
    
    mystl::priority_queue<int> foo,bar;
  foo.push (15); foo.push(30); foo.push(10);
  bar.push (101); bar.push(202);

  foo.swap(bar);

  std::cout << "size of foo: " << foo.size() << '\n';
  std::cout << "size of bar: " << bar.size() << '\n';
}

TEST(test8, top)
{
    mystl::priority_queue<int> mypq;

  mypq.push(10);
  mypq.push(20);
  mypq.push(15);

  std::cout << "mypq.top() is now " << mypq.top() << '\n';
}

TEST(test9, generic_swap)
{
    mystl::priority_queue<int> foo,bar;
  foo.push (15); foo.push(30); foo.push(10);
  bar.push (101); bar.push(202);

  swap(foo,bar);

  std::cout << "size of foo: " << foo.size() << '\n';
  std::cout << "size of bar: " << bar.size() << '\n';

}