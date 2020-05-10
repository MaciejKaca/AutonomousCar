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
        bool validateSpeed(const U16 &_speed);
        void checkAndStopThread();
        void constantMovement();
        void acceleration();

        bool threadStatus;
        std::thread stepperThread;
        U16 desiredSpeed;
        U16 speed;
        StepperMotorDirection direction;

        const U16 MAX_DELAY = 2000;
        const U16 MIN_DELAY = 300;
        const U8 STEPPER_ENABLE_PIN = 27;
        const U8 STEPPER_STEP_PIN = 29;
        const U8 STEPPER_DIRECTION_PIN = 28;
        const U8 X_AXIS_OFFSET = 1;
        const U8 ACCEL_STEP_RESOLUTION = 1;
        const U8 ACCEL_FUNC_SWITCH_THRES = 225;
        const float ACCELERATION_LOG = 12.5;
        const float ACCELERATION_LINEAR = 0.3;
};
