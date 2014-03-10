set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

qt5_wrap_cpp(QtGA_SRC ${QtGA_HEADERS})

add_library(QtGA SHARED QtGA.cpp ${QtGA_SRC})
qt5_use_modules(QtGA Core)
