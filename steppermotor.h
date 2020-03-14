#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "common.h"
#include "steppermotor.h"

#include <thread>

class StepperMotor
{
    public:
        StepperMotor();
        void move(const StepperMotorDirection direction, const U16 speed);
        void brake();
        void swithOff();
        bool makeStep(const StepperMotorDirection direction,  U16 speed);

    private:
        U16 speed;
        U16 delay;
        StepperMotorDirection direction;
        void validateSpeed(U16 & speed);
        std::thread stepperThread;
        void checkAndStopThread();
        void constantMovement();
        bool threadActive;
};

#endif // STEPPERMOTOR_H
