#pragma once

#include "common.h"
#include "Base/StepperMotorBase.h"

#include <thread>

class StepperMotor : StepperMotorBase
{
    public:
        StepperMotor();
        ~StepperMotor();
        void move(const StepperMotorDirection direction, const U16 _speed) override;
        void brake() override;
        void switchOff() override;
        bool makeStep(const StepperMotorDirection direction, const U16 _speed) override;
        const bool &isThreadActive() override;
        const U16 &getSpeed() override;
        const StepperMotorDirection &getDirection() override;
        static const U16 MAX_SPEED = 100;
        static const U16 MIN_SPEED = 0;

    private:
        U16 speed;
        StepperMotorDirection direction;
        bool validateSpeed(const U16 &_speed);
        std::thread stepperThread;
        void checkAndStopThread();
        void constantMovement();
        bool threadStatus;

        const U16 MAX_DELAY = 2000;
        const U16 MIN_DELAY = 300;
        const U8 STEPPER_ENABLE_PIN = 27;
        const U8 STEPPER_STEP_PIN = 29;
        const U8 STEPPER_DIRECTION_PIN = 28;

};
