#pragma once

#include "common.h"
#include "Base/StepperMotorBase.h"

#include <thread>

class StepperMotor : StepperMotorBase
{
    public:
        StepperMotor();
        void move(const StepperMotorDirection direction, const U16 speed) override;
        void brake() override;
        void swithOff() override;
        bool makeStep(const StepperMotorDirection direction,  U16 speed) override;
        const bool &isThreadActive() override;
        const U16 &getSpeed() override;
        const StepperMotorDirection &getDirection() override;

    private:
        U16 speed;
        U16 delay;
        StepperMotorDirection direction;
        void validateSpeed(U16 & speed);
        std::thread stepperThread;
        void checkAndStopThread();
        void constantMovement();
        bool threadStatus;
};
