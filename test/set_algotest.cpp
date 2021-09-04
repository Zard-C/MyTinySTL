// set_algotest.cpp -- test codes for set_algo
#include <gtest/gtest.h>  
#include "set_algo.h" 
#include <vector>  
#include <algorithm> 


TEST(test1, set_union)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_union (first, first+5, second, second+5, v.begin());
                                                // 5 10 15 20 25 30 40 50  0  0
    v.resize(it-v.begin());                      // 5 10 15 20 25 30 40 50

    std::cout << "The union has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

struct myless
{
public:  
   bool operator() (int a, int b) { return a < b; }
};

TEST(test2, set_union_generic)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_union (first, first+5, second, second+5, v.begin(), myless());
                                                // 5 10 15 20 25 30 40 50  0  0
    v.resize(it-v.begin());                      // 5 10 15 20 25 30 40 50

    std::cout << "The union has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test3, set_intersection)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_intersection (first, first+5, second, second+5, v.begin());
                                                 // 10 20 0  0  0  0  0  0  0  0
    v.resize(it-v.begin());                      // 10 20

    std::cout << "The intersection has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}

TEST(test4, set_intersection_generic)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_intersection (first, first+5, second, second+5, v.begin(), myless());
                                                 // 10 20 0  0  0  0  0  0  0  0
    v.resize(it-v.begin());                      // 10 20

    std::cout << "The intersection has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}

TEST(test5, set_difference)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_difference (first, first+5, second, second+5, v.begin());
                                                //  5 15 25  0  0  0  0  0  0  0
    v.resize(it-v.begin());                      //  5 15 25

    std::cout << "The difference has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test6, set_difference_generic)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_difference (first, first+5, second, second+5, v.begin(), myless());
                                                //  5 15 25  0  0  0  0  0  0  0
    v.resize(it-v.begin());                      //  5 15 25

    std::cout << "The difference has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
    std::cout << '\n'; 
}

TEST(test7, set_symmetric_difference)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_symmetric_difference (first, first+5, second, second+5, v.begin());
                                                //  5 15 25 30 40 50  0  0  0  0
    v.resize(it-v.begin());                      //  5 15 25 30 40 50

    std::cout << "The symmetric difference has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

TEST(test8, set_symmetric_difference_generic)
{
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it;

    std::sort (first,first+5);     //  5 10 15 20 25
    std::sort (second,second+5);   // 10 20 30 40 50

    it=mystl::set_symmetric_difference (first, first+5, second, second+5, v.begin(), myless());
                                                //  5 15 25 30 40 50  0  0  0  0
    v.resize(it-v.begin());                      //  5 15 25 30 40 50

    std::cout << "The symmetric difference has " << (v.size()) << " elements:\n";
    for (it=v.begin(); it!=v.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}