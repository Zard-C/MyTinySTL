#include <gtest/gtest.h> 
#include <algorithm>
#include <iostream> 
#include <utility> 
#include <string> 
#include "functional.h"

TEST(test1, operators)
{
    int res = mystl::plus<int>()(2, 3); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 5); 

    res = mystl::minus<int>()(2, 3); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, -1); 

    res = mystl::multiplies<int>()(2, 3); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 6); 

    res = mystl::divides<int>()(5, 3); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 1); 

    res = mystl::modulus<int>()(5, 3); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 2); 

    res = mystl::negate<int>()(5); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, -5); 

    res = identity_element(mystl::plus<int>()); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 0); 

    res = identity_element(mystl::multiplies<int>()); 
    std::cout << res << std:: endl; 
    EXPECT_EQ(res, 1);   


    bool result = true; 
    result = mystl::equal_to<int>()(5, 3); 
    EXPECT_EQ(result, false); 
 
    result = mystl::not_equal_to<int>()(5, 3); 
    EXPECT_EQ(result, true); 

    result = mystl::greater<int>()(5, 3);  
    EXPECT_EQ(result, true); 

    result = mystl::greater<int>()(0, 3); 
    EXPECT_EQ(result, false);
    
    result = mystl::greater<int>()(3, 3); 
    EXPECT_EQ(result, false);

    result = mystl::less<int>()(0, 3); 
    EXPECT_EQ(result, true);

    result = mystl::less<int>()(5, 3); 
    EXPECT_EQ(result, false);

    result = mystl::greater_equal<int>()(3, 3); 
    EXPECT_EQ(result, true);

    result = mystl::greater_equal<int>()(2, 3); 
    EXPECT_EQ(result, false);

    result = mystl::less_equal<int>()(3, 3); 
    EXPECT_EQ(result, true);

    result = mystl::less_equal<int>()(5, 3); 
    EXPECT_EQ(result, false);   

    result = mystl::logical_and<bool>()(true, true); 
    EXPECT_EQ(result, true);

    result = mystl::logical_and<bool>()(true, false); 
    EXPECT_EQ(result, false);

    result = mystl::logical_or<bool>()(true, false); 
    EXPECT_EQ(result, true);

    result = mystl::logical_or<bool>()(false, false); 
    EXPECT_EQ(result, false);

    result = mystl::logical_not<bool>()(true); 
    EXPECT_EQ(result, false);

    result = mystl::logical_not<bool>()(false); 
    EXPECT_EQ(result, true);
}

TEST(test2, identity)
{
    std::pair<int, std::string> mypair(2021, "c++"); 
    std::string mystring = "rocks"; 

    int a = 2021; 

    auto p = mystl::identity<int>()(a); 
    std::cout << typeid(p).name() <<std::endl; 
    std::cout << p << std::endl;  

    a = mystl::selectfirst<std::pair<int, std::string>>()(mypair); 
    std::cout << a << std:: endl; 

    mystring = mystl::selectsecond<std::pair<int, std::string>>()(mypair); 
    std::cout << mystring << std::endl; 


    a = mystl::projectfirst<int, std::string>()(a, mystring); 
    std::cout << a << std::endl; 
    EXPECT_EQ(a, 2021); 

    mystring = mystl::projectsecond<int, std::string>()(a, mystring); 
    std::cout << mystring << std::endl; 
    EXPECT_EQ(mystring, "c++");
}