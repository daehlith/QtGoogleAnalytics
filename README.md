What is QtGA ?
--------------

QtGA is C++ library for integrating Google Analytics in a Qt application.

Dependencies
------------
To build QtGA from source you will need
[CMake](http://www.cmake.org) (>=2.8), and
[Qt](http://www.qt-project.org) (>=5.1).

Note that building *might* work with earlier versions of CMake and Qt, but I have not tested this.

If you want to use SSL encryption in QtGA please ensure that Qt was build with SSL support.

Building
--------
Building is simple and straight forward. As with CMake best practices, an out-of-source build is recommended.

To build the library simply run this from the project root:

    mkdir build
    cd build
    cmake ..
    make
