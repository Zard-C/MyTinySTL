#include <gtest/gtest.h> 
#include "vector.h"
#include <vector> 


TEST(test1, create_vector)
{
    mystl::vector<int> vec; 
    std::cout << "vector<T> size is " << sizeof(vec) << std::endl; 
    EXPECT_EQ(sizeof(vec), 3*sizeof(void* )); 
    std::cout << "vec.size() = " << vec.size() << "\n"; 
    std::cout << "vec.capacity() = " << vec.capacity() << "\n"; 

}

// memory leak ! ? 没写~vector()
TEST(test2, constructor)
{
    // constructors used in the same order as described above:
    mystl::vector<int> first;                                // empty vector of ints

    // fill 
    mystl::vector<int> second (4,100);                       // four ints with value 100

    // range 
    mystl::vector<int> third (second.begin(),second.end());  // iterating through second

    // copy constructor 
    mystl::vector<int> fourth (third);                       // a copy of third



    // the iterator constructor can also be used to construct from arrays:
    int myints[8] = {16,2,77,29};
    mystl::vector<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

    std::cout << "The contents of fifth are:";
    for (mystl::vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

    for(auto ite = fourth.begin(); ite != fourth.end(); ++ite)
    {
        std::cout << *ite << " "; 
    }
    std::cout << "\n"; 

    // copy ctor
    first = fifth; 
    for(auto ite = first.begin(); ite != first.end(); ++ite)
    {
        std::cout << *ite << " "; 
    }
    std::cout << "\n"; 

    // opeartor == 
    EXPECT_EQ(first == fifth, true); 
    std::cout << "first.size()= "<<first.size()<<" " <<"first.capacity()= " 
        <<first.capacity() << "\n" ; 
    std::cout << "fifth.size()= "<<fifth.size()<<" " <<"fifth.capacity()= "
        <<fifth.capacity() << "\n";

    std::cout << first.capacity() + second.capacity() + third.capacity() + fourth.capacity() + fifth.capacity() 
        <<"\n"; 
}

TEST(test3, assign)
{
    mystl::vector<int> first;
    mystl::vector<int> second;
    mystl::vector<int> third;

    first.assign (7,100);             // 7 ints with a value of 100

    mystl::vector<int>::iterator it;
    it=first.begin()+1;

    second.assign (it,first.end()-1); // the 5 central values of first

    int myints[] = {1776,7,4};
    third.assign (myints,myints+3);   // assigning from array.

    std::cout << "Size of first: " << int (first.size()) << '\n';
    std::cout << "Size of second: " << int (second.size()) << '\n';
    std::cout << "Size of third: " << int (third.size()) << '\n';
}

TEST(test4, at)
{
    // at:  with bound-check 
    mystl::vector<int> myvector (10);   // 10 zero-initialized ints

    // assign some values:
    for (unsigned i=0; i<myvector.size(); i++)
        myvector.at(i)=i;

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector.at(i);
    std::cout << '\n';
    // with bound check - exception
    // myvector.at(11); 
    // [] no bound check - about 
    //myvector[11];
}

TEST(test5, back)
{
    mystl::vector<int> myvector;

    myvector.push_back(10);

    while (myvector.back() != 0)
    {
        myvector.push_back ( myvector.back() -1 );
    }

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size() ; i++)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';

}

TEST(test6, begin)
{

    mystl::vector<int> myvector;
    for (int i=1; i<=5; i++) myvector.push_back(i);

    std::cout << "myvector contains:";
    for (mystl::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test7, capacity)
{
    // 使用的不是加倍策略，而是1.5倍策略
    mystl::vector<int> myvector;

    // set some content in the vector:
    for (int i=0; i<100; i++)
    {
        myvector.push_back(i);
        //std::cout << "capacity: " << (int) myvector.capacity() <<" ";
    }
    std::cout << "\n";
    std::cout << "size: " << (int) myvector.size() << '\n';
    std::cout << "capacity: " << (int) myvector.capacity() << '\n';
    std::cout << "max_size: " <<  myvector.max_size() << '\n';

}

TEST(test8, cbegin_cend)
{
    // const_iterator
    mystl::vector<int> myvector = {10,20,30,40,50};

    std::cout << "myvector contains:";

    for (auto it = myvector.cbegin(); it != myvector.cend(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test9, clear)
{
    mystl::vector<int> myvector;
    myvector.push_back (100);
    myvector.push_back (200);
    myvector.push_back (300);

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';

    myvector.clear();
    myvector.push_back (1101);
    myvector.push_back (2202);

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';
}

TEST(test10, crbegin_crend)
{
    mystl::vector<int> myvector = {1,2,3,4,5};

    std::cout << "myvector backwards:";
    for (auto rit = myvector.crbegin(); rit != myvector.crend(); ++rit)
        std::cout << ' ' << *rit;
    std::cout << '\n';
}

TEST(test11, data)
{
    mystl::vector<int> myvector (5);

    int* p = myvector.data();

    *p = 10;
    ++p;
    *p = 20;
    p[2] = 100;

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); ++i)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';
    // 10 20 0 100 0
}

// emplace -> 
TEST(test12, emplace)
{
    mystl::vector<int> myvector = {10,20,30};

    auto it = myvector.emplace ( myvector.begin()+1, 100 );
    std::cout << myvector.capacity() << "\n";
    myvector.emplace ( it, 200 );
    myvector.emplace ( myvector.end(), 300 );

    std::cout << "myvector contains:";
    for (auto& x: myvector)
        std::cout << ' ' << x;
    std::cout << '\n';
    // 10 200 100 20 30 300
}

// +++ 添加元素后 _end 没有更新 
TEST(test13, emplace_back)
{
    std::vector<int> myvector = {10,20,30};

    myvector.emplace_back (100);
    myvector.emplace_back (200);


    std::cout << "myvector contains:";
    for (auto& x: myvector)
        std::cout << ' ' << x;
    std::cout << '\n';
}

TEST(test14, empty)
{
    mystl::vector<int> myvector;
    int sum (0);

    for (int i=1;i<=10;i++) myvector.push_back(i);

    while (!myvector.empty())
    {
        sum += myvector.back();
        myvector.pop_back();
    }

    std::cout << "total: " << sum << '\n';
}

TEST(test15, end)
{
    mystl::vector<int> myvector;
    for (int i=1; i<=5; i++) myvector.push_back(i);

    std::cout << "myvector contains:";
    for (mystl::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test16, erase)
{
    mystl::vector<int> myvector;

    // set some values (from 1 to 10)
    for (int i=1; i<=10; i++) myvector.push_back(i);

    // erase the 6th element
    myvector.erase (myvector.begin()+5);

    // erase the first 3 elements:
    myvector.erase (myvector.begin(),myvector.begin()+3);

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); ++i)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';
}

TEST(test17, front)
{
    mystl::vector<int> myvector;

    myvector.push_back(78);
    myvector.push_back(16);

    // now front equals 78, and back 16

    myvector.front() -= myvector.back();

    std::cout << "myvector.front() is now " << myvector.front() << '\n';
}

TEST(test18, get_allocator)
{
    mystl::vector<int> myvector;
    int * p;
    unsigned int i;

    // allocate an array with space for 5 elements using vector's allocator:
    p = myvector.get_allocator().allocate(5);

    // construct values in-place on the array:
    for (i=0; i<5; i++) myvector.get_allocator().construct(&p[i],i);

    std::cout << "The allocated array contains:";
    for (i=0; i<5; i++) std::cout << ' ' << p[i];
    std::cout << '\n';

    // destroy and deallocate:
    for (i=0; i<5; i++) myvector.get_allocator().destroy(&p[i]);
    myvector.get_allocator().deallocate(p,5);
}

TEST(test19, insert)
{
    mystl::vector<int> myvector (3,100);
    mystl::vector<int>::iterator it;

    it = myvector.begin();
    it = myvector.insert ( it , 200 );

    // 没保存返回的iterator
    myvector.insert (it,2,300);

    // "it" no longer valid, get a new one:
    it = myvector.begin();

    std::vector<int> anothervector (2,400);
    myvector.insert (it+2,anothervector.begin(),anothervector.end());

    int myarray [] = { 501,502,503 };
    myvector.insert (myvector.begin(), myarray, myarray+3);

    std::cout << "myvector contains:";
    for (it=myvector.begin(); it<myvector.end(); it++)
        std::cout << ' ' << *it;
    std::cout << '\n';
    //501 502 503 300 300 400 400 200 100 100 100
}

TEST(test20, operator_equal)
{
    // operator=
    mystl::vector<int> foo (3,0);
    mystl::vector<int> bar (5,0);

    bar = foo;
    foo = mystl::vector<int>();

    std::cout << "Size of foo: " << int(foo.size()) << '\n';
    std::cout << "Size of bar: " << int(bar.size()) << '\n';
    EXPECT_EQ(foo.size(), 0); 
    EXPECT_EQ(bar.size(), 3);
}

TEST(test21, operator_index)
{
    mystl::vector<int> myvector (10);   // 10 zero-initialized elements

    mystl::vector<int>::size_type sz = myvector.size();

    // assign some values:
    for (unsigned i=0; i<sz; i++) myvector[i]=i;

    // reverse vector using operator[]:
    for (unsigned i=0; i<sz/2; i++)
    {
        int temp;
        temp = myvector[sz-1-i];
        myvector[sz-1-i]=myvector[i];
        myvector[i]=temp;
    }

    std::cout << "myvector contains:";
    for (unsigned i=0; i<sz; i++)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';
}

TEST(test22, pop_back)
{
    mystl::vector<int> myvector;
    int sum (0);
    myvector.push_back (100);
    myvector.push_back (200);
    myvector.push_back (300);

    while (!myvector.empty())
    {
        sum+=myvector.back();
        myvector.pop_back();
    }

    std::cout << "The elements of myvector add up to " << sum << '\n';
}

TEST(test23, push_back)
{
    mystl::vector<int> vec; 
    for(int i = 0; i < 5; ++i) 
    {
        vec.push_back(i); 
    }
    std::cout << "myvector contains: "; 
    for(int& x : vec)
    {
        std::cout << x <<" "; 
    }
    std::cout << "\n"; 
}

TEST(test24, rbegin_rend)
{
    mystl::vector<int> myvector (5);  // 5 default-constructed ints

    int i=0;

    mystl::vector<int>::reverse_iterator rit = myvector.rbegin();
    for (; rit!= myvector.rend(); ++rit)
        *rit = ++i;

    std::cout << "myvector contains:";
    for (mystl::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test25, reserve)
{
    mystl::vector<int>::size_type sz;

    mystl::vector<int> foo;
    sz = foo.capacity();
    std::cout << "making foo grow:\n";
    for (int i=0; i<100; ++i) {
        foo.push_back(i);
        if (sz!=foo.capacity()) {
            sz = foo.capacity();
            std::cout << "capacity changed: " << sz << '\n';
        }
    }

    mystl::vector<int> bar;
    sz = bar.capacity();
    bar.reserve(100);   // this is the only difference with foo above
    std::cout << "making bar grow:\n";
    for (int i=0; i<100; ++i) {
        bar.push_back(i);
        if (sz!=bar.capacity()) {
            sz = bar.capacity();
            std::cout << "capacity changed: " << sz << '\n';
        }
    }
}

TEST(test26, resize)
{
    mystl::vector<int> myvector;

    // set some initial content:
    for (int i=1;i<10;i++) myvector.push_back(i);

    myvector.resize(5);
    myvector.resize(8,100);
    myvector.resize(12);

    std::cout << "myvector contains:";
    for (int i=0;i<myvector.size();i++)
    std::cout << ' ' << myvector[i];
    std::cout << '\n';
}

TEST(test27, shrink_to_fit)
{
    mystl::vector<int> myvector (100);
    std::cout << "1. capacity of myvector: " << myvector.capacity() << '\n';

    myvector.resize(10);
    std::cout << "2. capacity of myvector: " << myvector.capacity() << '\n';

    myvector.shrink_to_fit();
    std::cout << "3. capacity of myvector: " << myvector.capacity() << '\n';
}

TEST(test28, size)
{
    mystl::vector<int> myints;
    std::cout << "0. size: " << myints.size() << '\n';

    for (int i=0; i<10; i++) myints.push_back(i);
    std::cout << "1. size: " << myints.size() << '\n';

    myints.insert (myints.end(),10,100);
    std::cout << "2. size: " << myints.size() << '\n';

    myints.pop_back();
    std::cout << "3. size: " << myints.size() << '\n';
}

TEST(test29, swap)
{
    mystl::vector<int> foo (3,100);   // three ints with a value of 100
    mystl::vector<int> bar (5,200);   // five ints with a value of 200

    foo.swap(bar);

    std::cout << "foo contains:";
    for (unsigned i=0; i<foo.size(); i++)
    std::cout << ' ' << foo[i];
    std::cout << '\n';

    std::cout << "bar contains:";
    for (unsigned i=0; i<bar.size(); i++)
    std::cout << ' ' << bar[i];
    std::cout << '\n';
}

TEST(test30, relational_operators)
{
    mystl::vector<int> foo (3,100);   // three ints with a value of 100
    mystl::vector<int> bar (2,200);   // two ints with a value of 200

    if (foo==bar) std::cout << "foo and bar are equal\n";
    if (foo!=bar) std::cout << "foo and bar are not equal\n";
    if (foo< bar) std::cout << "foo is less than bar\n";
    if (foo> bar) std::cout << "foo is greater than bar\n";
    if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
    if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
}

TEST(test31, general_swap)
{

    std::vector<int> foo (3,100);   // three ints with a value of 100
    std::vector<int> bar (5,200);   // five ints with a value of 200

    foo.swap(bar);

    std::cout << "foo contains:";
    for (std::vector<int>::iterator it = foo.begin(); it!=foo.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

    std::cout << "bar contains:";
    for (std::vector<int>::iterator it = bar.begin(); it!=bar.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}