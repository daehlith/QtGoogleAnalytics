find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

find_package(Qt5Test REQUIRED)
include_directories(${Qt5Test_INCLUDE_DIRS})
add_definitions(${Qt5Test_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Test_EXECUTABLE_COMPILE_FLAGS}")

qt5_wrap_cpp(QtGoogleAnalyticsTests_SRC testnetworkaccessmanager.h)

add_executable(tests tests.cpp testnetworkaccessmanager.cpp ${QtGoogleAnalyticsTests_SRC})
target_link_libraries(tests ${GTEST_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5Test_LIBRARIES} QtGoogleAnalytics)
