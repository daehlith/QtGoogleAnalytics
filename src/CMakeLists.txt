set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})

find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})

qt5_wrap_cpp(QtGoogleAnalytics_SRC ${QtGoogleAnalytics_HEADERS})

add_library(QtGoogleAnalytics SHARED QtGoogleAnalytics.cpp ${QtGoogleAnalytics_SRC})
target_link_libraries(QtGoogleAnalytics ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
