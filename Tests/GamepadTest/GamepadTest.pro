include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += qt console c++11 warn_on depend_includepath testcase
CONFIG -= app_bundle

HEADERS += \
        tst_testcasename.h

SOURCES += \
        main.cpp
