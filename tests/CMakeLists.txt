enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

add_executable(runTests tests.cpp)
target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES} ${Qt5Core_LIBRARIES} QtGA)
GTEST_ADD_TESTS(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/runTests "" tests.cpp)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS runTests)
