cmake_minimum_required(VERSION 2.8)

project(QtGA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/QtGA)
set(QtGA_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QtGA/QtGA.h)

add_subdirectory(src)
add_subdirectory(tests)
