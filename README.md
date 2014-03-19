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

Additionally, the tests require [Google Test Framework](http://googletest.googlecode.com) (>=1.70).

Note that building *might* work with earlier versions of those tools and libraries, but I have not tested this.

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

Assuming the above build instructions were used, this is how you compile and run the tests:

    make tests
    ../bin/tests

Future Improvements
-------------------
Though this library is already usable it is still in very early development. This is nothing bad per se, but it also
means that there is room for improvements. In this section I am keeping track of what is bothering me at the moment. Partly to keep ideas for future blogs about this lib.

 - Validation is not quite there yet. Some of the most obvious problems are ruled out, but I really want to add
   some form of making sure that hits contain all required parameters, and that all values are as expected.
 - Error reporting is lacking. Printing messages through Qt's message handler is OK for now, but some form of error
   signal should be available so that users of this library can respond to those conditions, if they want to.
 - The if-else switch based on the QNetworkAccessManager::Operation looks weird. This isn't a runtime choice, so
   this could probably be replaced with some form of policy where templates get evaluated at compile time.
 - Add support for something like "prepared hits", e.g. a way where some sort of hit template can be specified
   and only very few parameters need to get added.
 - Look into some small optimization with regards to construction of the QNetworkRequest in track. There is no need to
   always do all the work, even though that does somewhat depend on the request method we are using.
