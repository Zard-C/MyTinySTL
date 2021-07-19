#include <gtest/gtest.h> 
#include "../include/memory.h"
#include <iterator> 
bool test01(void);
bool test02(void);
bool test03(void); 
bool  test04(void);

TEST(test1, addressof)
{
    ASSERT_EQ(test01(), true);
}

TEST(test2, get_temporary_buffer)
{
    ASSERT_EQ(test02(), true);
}

TEST(test3, get_temporary_buffer_cppreference)
{
    ASSERT_EQ(test03(), true);
}

TEST(test04, auto_ptr)
{
    ASSERT_EQ(test04(), true); 
}
