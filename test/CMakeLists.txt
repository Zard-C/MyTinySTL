# test 文件夹的构建规则

# 最低cmake 版本要求
cmake_minimum_required(VERSION 3.9)

# 定义This 变量 值为ExampleTest
set(This unitTest)

# 定义Sources 变量 值为ExampleTest.cpp 
set (Sources 
    rbtreetest.cpp
)

# 给工程设置名称
project(${This} C CXX)

# 相当于 g++ ${Sources} -o ${This} 
add_executable(${This} ${Sources})

# 指定链接库， 参数都是库的名称
target_link_libraries(${This} PUBLIC 
    gtest_main
    mystltest
)

# 添加一个测试
add_test(
    NAME ${This}
    COMMAND ${This}
)