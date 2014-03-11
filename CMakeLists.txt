cmake_minimum_required(VERSION 2.8)
if(POLICY CMP0020)
        cmake_policy(SET CMP0020 NEW) # Drop when CMake >= 2.8.11 required
endif()

project(QtGA)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/QtGA)
set(QtGA_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QtGA/QtGA.h)

add_subdirectory(src)
add_subdirectory(tests)
