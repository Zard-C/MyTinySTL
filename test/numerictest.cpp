// -- numerictest.cpp 测试样例from www.cppreference.com 
#include <gtest/gtest.h> 
#include "numeric.h"
#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <vector> 
#include <list> 
#include <random> 
#include <algorithm>


int myfunction (int x, int y) {return x+2*y;}
struct myclass 
{
	int operator()(int x, int y) {return x+3*y;}
} myobject;

TEST(test1, accumulate)
{
    int init = 100;
    int numbers[] = {10,20,30};

    std::cout << "using default accumulate: ";
    std::cout << mystl::accumulate(numbers,numbers+3,init);
    std::cout << '\n';

    std::cout << "using functional's minus: ";
    std::cout << mystl::accumulate (numbers, numbers+3, init, std::minus<int>());
    std::cout << '\n';

    std::cout << "using custom function: ";
    std::cout << mystl::accumulate (numbers, numbers+3, init, myfunction);
    std::cout << '\n';

    std::cout << "using custom object: ";
    std::cout << mystl::accumulate (numbers, numbers+3, init, myobject);
    std::cout << '\n';

    int res = 0; 
    std::vector<int> v{10, 20, 30}; 
        std::cout << "using default accumulate: ";
    res = mystl::accumulate(v.begin(), v.end(), init);
    std::cout << res;
    std::cout << '\n';
    EXPECT_EQ(res, 160);

    std::cout << "using functional's minus: ";
    res =  mystl::accumulate (v.begin(), v.end(), init, std::minus<int>());
    std::cout <<res;
    std::cout << '\n';
    EXPECT_EQ(res, 40);

    std::cout << "using custom function: ";
    res = mystl::accumulate (v.begin(), v.end(), init, myfunction);
    std::cout << res;
    std::cout << '\n';
    EXPECT_EQ(res, 220);

    std::cout << "using custom object: ";
    res = mystl::accumulate (v.begin(), v.end(), init, myobject);
    std::cout << res; 
    std::cout << '\n';
    EXPECT_EQ(res, 280);
}

TEST(test2, adjacent_difference)
{
    std::vector<int> v{2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    mystl::adjacent_difference(v.begin(), v.end(), v.begin());
 
    for(auto n: v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
 
    //fibonacci 
    v = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    mystl::adjacent_difference(v.begin(), v.end()-1, v.begin()+1, std::plus<int>());
 
    for(auto n: v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}


static int myaccumulator (int x, int y) {return x-y;}
static int myproduct (int x, int y) {return x+y;}
TEST(test3, inner_product)
{
    int init = 100;
    int series1[] = {10,20,30};
    int series2[] = {1,2,3};
    int res = 0; 

    std::cout << "using default inner_product: ";
    res = mystl::inner_product(series1,series1+3,series2,init);
    std::cout << res; 
    std::cout << '\n';
    EXPECT_EQ(res, 240);

    std::cout << "using functional operations: ";
    res = mystl::inner_product(series1,series1+3,series2,init,
                                    std::minus<int>(),std::divides<int>());
    std::cout << res; 
    std::cout << '\n';
    EXPECT_EQ(res, 70);

    std::cout << "using custom functions: ";
    res = mystl::inner_product(series1,series1+3,series2,init,
                                    myaccumulator,myproduct);
    std::cout << res; 
    std::cout << '\n';
    EXPECT_EQ(res, 34); 
}

TEST(test4, iota)
{
    std::list<int> l(10);
    mystl::iota(l.begin(), l.end(), -4);

    // 放置的是list的迭代器
    std::vector<std::list<int>::iterator> v(l.size());
    mystl::iota(v.begin(), v.end(), l.begin());

    // 随机置乱算法 
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    std::cout << "Contents of the list: ";
    for(auto n: l) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "Contents of the list, shuffled: ";
    for(auto i: v) std::cout << *i << ' ';
    std::cout << '\n';
}


static int myop (int x, int y) {return x+y+1;}
TEST(test5, partial_sum) 
{
    int val[] = {1,2,3,4,5};
    int result[5];

    mystl::partial_sum (val, val+5, result);
    std::cout << "using default partial_sum: ";
    for (int i=0; i<5; i++) std::cout << result[i] << ' ';
    std::cout << '\n';

    mystl::partial_sum (val, val+5, result, std::multiplies<int>());
    std::cout << "using functional operation multiplies: ";
    for (int i=0; i<5; i++) std::cout << result[i] << ' ';
    std::cout << '\n';

    mystl::partial_sum (val, val+5, result, myop);
    std::cout << "using custom function: ";
    for (int i=0; i<5; i++) std::cout << result[i] << ' ';
    std::cout << '\n';
}


