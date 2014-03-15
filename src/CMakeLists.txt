set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

qt5_wrap_cpp(QtGoogleAnalytics_SRC ${QtGoogleAnalytics_HEADERS})

set(BUILD_SHARED_LIBS off)
if(BUILD_SHARED_LIBS)
    add_definitions(-DBUILD_SHARED)
endif()

add_library(QtGoogleAnalytics QtGoogleAnalytics.cpp ${QtGoogleAnalytics_SRC})
target_link_libraries(QtGoogleAnalytics ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
