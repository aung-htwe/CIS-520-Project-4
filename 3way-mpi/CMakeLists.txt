cmake_minimum_required(VERSION 3.5)
project(mpi)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -g")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Werror -g")


find_package(MPI REQUIRED)

add_executable(mpi_ascii main.c)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(mpi_ascii ${MPI_C_LIBRARIES})
