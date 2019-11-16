#pragma once

enum StepperMotorCommand
{
    RUN = 0,
    MOVE_DISTANCE = 1,
    SWICTH_OFF = 2,
    BRAKE = 3
};

enum StepperMotorDirection
{
    FORWARD = 0,
    BACKWARD = 1
};

struct StepperMotorMsg
{
    StepperMotorCommand command;
    StepperMotorDirection direction;
    unsigned short int speed;
};