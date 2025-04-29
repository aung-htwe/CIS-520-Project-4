cmake_minimum_required(VERSION 3.9)
project (openmp)

set(CMAKE_C_FLAGS "-std=c11 -Wall -g")
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g")

find_package(OpenMP REQUIRED)


add_executable(openmp_ascii main.c)
target_link_libraries(openmp_ascii PRIVATE OpenMP::OpenMP_CXX)



