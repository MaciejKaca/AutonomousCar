#-------------------------------------------------
#
# Project created by QtCreator 2019-12-21T22:15:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutonomousCar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/DistanceSensors.cpp \
        src/FileHandling.cpp \
    src/IMU.cpp \
        src/SafetySystem.cpp \
        src/StepperMotorShell.cpp \
        src/main.cpp \
        joystick/joystick.cc \
        src/gamepad.cpp \
        src/steppermotor.cpp \
        src/serialport.cpp \
        RS-232/rs232.c \
        src/servo.cpp \
        src/lights.cpp \
        src/logutils.cpp \
        src/SettingsWindow.cpp \

HEADERS += \
    inc/AHRS.h \
        inc/Base/* \
        inc/Base/SafetySystemBase.h \
        inc/Base/StepperMotorShellBase.h \
        inc/DistanceSensors.h \
        inc/FileHandling.h \
    inc/IMU.h \
        inc/SafetySystem.h \
        inc/StepperMotorShell.h \
        inc/common.h \
        inc/steppermotor.h \
        inc/serialport.h \
        inc/servo.h \
        inc/lights.h \
        inc/logutils.h \
        inc/gamepad.h \
        CarInterface/interface.h \
        RS-232/rs232.h \
        inc/SettingsWindow.h \

FORMS += \
        Forms/SettingsWindow.ui \

target.path = /home/pi
INSTALLS += target

LIBS += -lwiringPi \
