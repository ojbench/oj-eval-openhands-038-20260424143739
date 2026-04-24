cmake_minimum_required(VERSION 3.10)
project(vector_plus_plus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -mtune=native -Wall -Wextra")

add_executable(code main.cpp)
