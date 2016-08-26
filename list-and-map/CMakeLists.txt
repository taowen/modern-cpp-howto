cmake_minimum_required(VERSION 3.6)
project(list-and-map)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")


find_package(Boost 1.61.0)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

set(SOURCE_FILES main.cpp)
add_executable(list-and-map ${SOURCE_FILES})

if(Boost_FOUND)

    target_link_libraries(list-and-map ${Boost_LIBRARIES})

endif()