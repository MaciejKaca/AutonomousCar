#pragma once

#include <gmock/gmock.h>
#include <inc/Base/StepperMotorBase.h>

class StepperMotorMock : public StepperMotorBase
{
public:
    MOCK_METHOD(void, brake, (), (override));
    MOCK_METHOD(void, switchOff, (), (override));
    MOCK_METHOD(void, move, (const StepperMotorDirection _direction, const U16 _speed), (override));
    MOCK_METHOD(void, makeStep, (const StepperMotorDirection _direction, const U16 _speed), (override));
    MOCK_METHOD(const double&, getSpeed, (), (override));
    MOCK_METHOD(const StepperMotorDirection&, getDirection, (), (override));
};
