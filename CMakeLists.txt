cmake_minimum_required(VERSION 3.17)
project(PageRank)

set(CMAKE_CXX_STANDARD 14)

add_executable(pr pr.cpp)
add_executable(pr_delta pr_delta.cpp)
add_executable(pr_delta_prio pr_delta_prio.cpp)
add_executable(getN get_N.cpp)