cmake_minimum_required(VERSION 3.6)
project(list-and-map)

include_directories(_vendor/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")

set(SOURCE_FILES main.cpp)
add_executable(list-and-map ${SOURCE_FILES})