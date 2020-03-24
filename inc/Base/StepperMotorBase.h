#pragma once

#include <inc/common.h>

class StepperMotorBase
{
    public:
        virtual void move(const StepperMotorDirection direction, const U16 speed) = 0;
        virtual void brake() = 0;
        virtual void swithOff() = 0;
        virtual bool makeStep(const StepperMotorDirection direction,  U16 speed) = 0;
        virtual const bool &isThreadActive() = 0;
        virtual const U16 &getSpeed() = 0;
        virtual const StepperMotorDirection &getDirection() = 0;
};
