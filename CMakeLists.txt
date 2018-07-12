cmake_minimum_required(VERSION 3.7)
project(enkelt)

file(GLOB_RECURSE ENKELT_LIBS "include/*.*pp")
add_library(enkelt INTERFACE)
target_include_directories(enkelt INTERFACE "include/")
target_compile_features(enkelt INTERFACE cxx_std_17)



add_executable(enkelt_test "test.cpp")
target_link_libraries(enkelt_test enkelt)
