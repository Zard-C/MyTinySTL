#include <gtest/gtest.h> 
#include "exceptdef.h" 

TEST(test1, assert)
{
    assert(true);
}

TEST(test2, length_error)
{
    int len = 1; 
    THROW_LENGTH_ERROR_IF(len < 0, "length negative"); 
}

TEST(test3, out_of_range)
{
    int len = 5; 
    int index = 4; 

    THROW_OUT_OF_RANGE_IF((!(index < len) && (index >=0)), "out_of_range"); 
}

TEST(test4, rumtimeerror)
{
    int mallocSize = 32; 
    THROW_RUNTIME_ERROR_IF(mallocSize > 100000, "bad mallocsize ");

}

#if 0 
TEST(test5, bad_alloc)
{
    // 1. 往死里分配，肯定会抛出bad_alloc
    // 2. 内存分配回收过多导致内存碎片化 
    for(int i = 0; ; ++i)
    {
        int *p = new int [INT_MAX];
    }
}
#endif 