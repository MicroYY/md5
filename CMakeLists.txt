#cmake_minimum_required (VERSION 2.6)
project (md5)

add_library(md5 md5.cpp)
add_executable(md5_test test.cpp)
target_link_libraries(md5_test md5)