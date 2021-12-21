cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror)
add_executable(test test/test.cpp)