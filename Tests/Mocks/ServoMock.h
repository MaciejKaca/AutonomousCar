#pragma once

#include <gmock/gmock.h>
#include <inc/servo.h>

class ServoMock : public Servo
{
public:
    MOCK_METHOD(void, turn, (const S8 &_angle));
    MOCK_METHOD(void, setNewCenter, (const S8 &_angle));
    MOCK_METHOD(S8, getAngle, ());
};
