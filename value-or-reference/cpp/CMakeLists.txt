cmake_minimum_required(VERSION 3.6)
project(value-or-reference)

include_directories(_vendor/catch/include)
include_directories(_vendor/range-v3/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")
if (USE_MODULES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++ -fno-autolink -fmodules -fmodules-cache-path=${CMAKE_SOURCE_DIR}/build")
endif(USE_MODULES)
set(SOURCE_FILES main.cpp)
add_executable(value-or-reference ${SOURCE_FILES})

find_package (Threads)
target_link_libraries (value-or-reference ${CMAKE_THREAD_LIBS_INIT})
