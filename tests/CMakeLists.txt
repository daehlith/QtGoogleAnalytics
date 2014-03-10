enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Qt5Core REQUIRED)

add_executable(runTests tests.cpp)
target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES} QtGA)
qt5_use_modules(runTests Core)
add_test(NAME QtGATests COMMAND runTests)
