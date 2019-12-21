#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "common.h"
#include "steppermotor.h"

class StepperMotor
{
    public:
        StepperMotor();
        void move(StepperMotorDirection direction, uint16_t speed);
        void brake();
        void swithOff();
        void constantMovement();
        bool threadActive;
        bool makeStep(StepperMotorDirection direction, uint16_t speed);

    private:
        uint16_t speed;
        uint16_t delay;
        StepperMotorDirection direction;
        void validateSpeed(uint16_t & speed);
};

#endif // STEPPERMOTOR_H
