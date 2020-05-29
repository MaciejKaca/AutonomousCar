include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += qt console c++11 warn_on depend_includepath testcase
CONFIG -= app_bundle

HEADERS += \
        $$PWD/Mocks/** \
        $$PWD/Suites/**
SOURCES += \
        main.cpp \
        $$PWD/../joystick/joystick.cc \
        $$PWD/../RS-232/rs232.c \
        $$PWD/../src/lights.cpp \
        $$PWD/../src/gamepad.cpp \
        $$PWD/../src/servo.cpp \
        $$PWD/../src/serialport.cpp \
        $$PWD/../src/steppermotor.cpp \
        $$PWD/../src/logutils.cpp

LIBS += -lwiringPi \

INCLUDEPATH += \
    $$PWD/.. \