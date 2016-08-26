cmake_minimum_required(VERSION 3.6)
project(unit-testing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")

set(SOURCE_FILES main.cpp)
add_executable(unit-testing ${SOURCE_FILES})