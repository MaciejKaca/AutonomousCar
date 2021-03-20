#pragma once

#include <inc/common.h>

class StepperMotorBase
{
    public:
        virtual void move(const StepperMotorDirection _direction, const U16 _speed) = 0;
        virtual void brake() = 0;
        virtual void switchOff() = 0;
        virtual void makeStep(const StepperMotorDirection _direction, const U16 _speed) = 0;
        virtual const double &getSpeed() = 0;
        virtual const StepperMotorDirection &getDirection() = 0;
        virtual ~StepperMotorBase() = 0;
};
