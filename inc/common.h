#pragma once

#include <CarInterface/interface.h>
#include <QDebug>

#include <cstdint>
#include <cstdlib>
#include <cmath>

typedef uint16_t U16;
typedef int16_t S16;
typedef uint8_t U8;
typedef int8_t S8;

enum ExitReason
{
    EXIT_BY_BUTTON = 1,
    EXIT_BY_MISSING_GAMEPAD = 2,
    EXIT_BY_FAILED_GPIO = 3,
    EXIT_BY_MISSING_MODULE = 4
};

const S8 LIGHTS_AND_SERVO_PORT_BASE[] = "ttyUSB0";
const S8 LIGHTS_AND_SERVO_PORT_BACKUP[] = "ttyUSB1";

enum StepperMotorCommand
{
    RUN = 0,
    SWICTH_OFF = 1,
    BRAKE = 2,
    MAKE_STEP = 3
};

enum StepperMotorDirection
{
    FORWARD = 0,
    BACKWARD = 1
};
