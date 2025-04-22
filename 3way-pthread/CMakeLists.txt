cmake_minimum_required(VERSION 3.5)
project(pthread)


set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -g")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Werror -g")

add_executable(pthread_ascii main.c)
target_compile_options(pthread_ascii PRIVATE -O2  -pthread)

