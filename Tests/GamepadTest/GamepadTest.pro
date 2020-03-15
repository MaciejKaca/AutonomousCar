include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += qt console c++11 warn_on depend_includepath testcase
CONFIG -= app_bundle

HEADERS += \
        tst_testcasename.h

SOURCES += \
        main.cpp

INCLUDEPATH += \
    /home/ekacmac/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/include/c++/4.8.3 \
    /home/ekacmac/raspi/sysroot/usr/include
