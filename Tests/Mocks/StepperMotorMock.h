#pragma once

#include <gmock/gmock.h>
#include <inc/steppermotor.h>

class StepperMotorMock : public StepperMotor
{
public:
    MOCK_METHOD(void, brake, ());
    MOCK_METHOD(void, switchOff, ());
    MOCK_METHOD(void, move, (const StepperMotorDirection direction, const U16 speed));
    MOCK_METHOD(bool, isThreadActive, ());
    MOCK_METHOD(U16, getSpeed, ());
    MOCK_METHOD(StepperMotorDirection, getDirection, ());
};
