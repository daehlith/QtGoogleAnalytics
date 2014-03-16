What is QtGoogleAnalytics ?
---------------------------

QtGoogleAnalytics is a C++ library for integrating Google Analytics in a Qt application.

The library aims at simplifying the HTTP requests involved for sending data to the Google servers via the
[Google Analytics Measurement Protocol](https://developers.google.com/analytics/devguides/collection/protocol/v1/).
Special care is being taken to ensure that no invalid requests are going to be sent, especially since Google 
Analytics will not report any errors on malformed requests, which can make debugging rather tedious.

Dependencies
------------
To build QtGoogleAnalytics from source you will need
[CMake](http://www.cmake.org) (>=2.8), and
[Qt](http://www.qt-project.org) (>=5.1).

Note that building *might* work with earlier versions of CMake and Qt, but I have not tested this.

If you want to use SSL encryption in QtGoogleAnalytics please ensure that Qt was build with SSL support.

Building
--------
Building is simple and straight forward. As with CMake best practices, an out-of-source build is recommended.

To build the library simply run this from the project root:

    mkdir .build
    cd .build
    cmake ..
    make

Testing
-------
QtGoogleAnalytics ships with a set of automated tests.

Assuming the outlied build instructions were used, this is how you build and run the tests:

    make tests
    ../bin/tests
