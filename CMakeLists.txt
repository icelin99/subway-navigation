cmake_minimum_required(VERSION 3.17)
project(pj2)

set(CMAKE_CXX_STANDARD 14)

# 添加python文件路径
include_directories(/usr/local/Cellar/python@3.9/3.9.9/Frameworks/Python.framework/Versions/3.9/include)
# 添加python动态链接库文件目录
link_directories(/usr/local/Cellar/python@3.9/3.9.9/Frameworks/Python.framework/Versions/3.9/lib/python3.9)

add_executable(pj2 main.cpp)
add_executable(bell bellman_ford.cpp bellman_ford.h)