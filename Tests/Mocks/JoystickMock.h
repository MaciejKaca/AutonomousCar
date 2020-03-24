#pragma once

#include <gmock/gmock.h>
#include <joystick/joystick.hh>

class JoystickMock : public Joystick
{
public:
    MOCK_METHOD(bool, isFound, ());
    MOCK_METHOD(bool, sample, (JoystickEvent* event));
};

class JoystickEventMock : public JoystickEvent
{
public:
    MOCK_METHOD(bool, isButton, ());
    MOCK_METHOD(bool, isAxis, ());
};
