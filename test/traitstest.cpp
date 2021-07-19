#include <iostream>
#include "type_traits.h"
#include <gtest/gtest.h> 
#include "util.h"


TEST(test1, traits)
{
    using mystl::pair; 
    using std::cout; 
    using std::endl; 
    using namespace mystl; 

    std::cout << m_bool_constant<true>::value << endl; 
    std::cout << m_bool_constant<false>::value << endl; 
    std::cout << typeid(m_bool_constant<true>::value_type).name() << endl;

    int a = 0; 
    int b = 0;  
    cout << mystl::is_pair<int>::value << endl; 
    cout << mystl::is_pair<pair<int,double>>::value << endl; 
}


