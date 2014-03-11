set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})

find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})

qt5_wrap_cpp(QtGA_SRC ${QtGA_HEADERS})

add_library(QtGA SHARED QtGA.cpp ${QtGA_SRC})
target_link_libraries(QtGA ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES})
