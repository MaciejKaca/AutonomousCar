#pragma once

#include <inc/common.h>

class StepperMotorBase
{
    public:
        virtual void move(const StepperMotorDirection _direction, const U16 _speed) = 0;
        virtual void brake() = 0;
        virtual void switchOff() = 0;
        virtual bool makeStep(const StepperMotorDirection _direction, const U16 _speed) = 0;
        virtual const bool &isThreadActive() = 0;
        virtual const U16 &getSpeed() = 0;
        virtual const StepperMotorDirection &getDirection() = 0;
};
