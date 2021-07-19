# 目标文件夹的构建规则

# 设置最低版本要求
cmake_minimum_required(VERSION 3.9)

# 定义This 变量， 值为 Example
set (This mystltest)

# 设置工程名为This 的值即Example
project(${This} C CXX)

# 指定语言标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
# 
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 添加include目录
include_directories(include)


# 启用测试
enable_testing()

# 要测试的源文件

set(
    SOURCES
    source/test.cpp
)

# 生成静态库
add_library(${This} STATIC ${SOURCES})

# 添加子文件夹
add_subdirectory(test)
add_subdirectory(googletest)

# add_executable(${This} ${Sources})
