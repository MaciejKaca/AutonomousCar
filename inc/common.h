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

const U8 GAMEPAD_REFRESH_TIME = 1; //Time in milliseconds

const U8 EXIT_BY_BUTTON = 1;
const U8 EXIT_BY_MISSING_GAMEPAD = 2;
const U8 EXIT_BY_FAILED_GPIO = 3;
const U8 EXIT_BY_MISSING_MODULE = 4;

const bool BUTTON_DOWN = true;
const bool BUTTON_UP = false;

const U8 NUMBER_OF_BUTTONS = 10;
const U8 NUMBER_OF_AXIS = 7;
const S16 MAX_AXIS_VALUE = 32767;
const S16 MIN_AXIS_VALUE = -32767;

enum ButtonID
{
    EXIT_BUTTON = 7,
    X_BUTTON = 2,
    RIGHT_BUMPER = 5,
    LEFT_BUMPER = 4,
};

enum AxisID
{
    LEFT_X_AXIS = 0,
    RIGHT_TRIGGER = 4,
    LEFT_TRIGGER = 5,
    ARROW_Y_AXIS = 7
};

const S8  LIGHTS_AND_SERVO_PORT [] = "ttyUSB0";
const S8  LIGHTS_AND_SERVO_PORT_BACKUP [] = "ttyUSB1";

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

const U16 MAX_SPEED = 2000;
const U16 MIN_SPEED = 300;

const U8 STEPPER_ENABLE_PIN = 27;
const U8 STEPPER_STEP_PIN = 29;
const U8 STEPPER_DIRECTION_PIN = 28;

const U8 TOTAL_WHEEL_ANGLE = 80;

static const float AXIS_TO_SPEED_SCALE =  float( std::abs(MIN_AXIS_VALUE) + MAX_AXIS_VALUE + 1 ) / float(MAX_SPEED - MIN_SPEED);
static const float AXIS_TO_DEEGRES_SCALE =  float( std::abs(MIN_AXIS_VALUE) + MAX_AXIS_VALUE + 1 ) / float(TOTAL_WHEEL_ANGLE);

static const S16 AXIS_MIN_TRIGGER_POSITION = MIN_AXIS_VALUE + static_cast<S16>(AXIS_TO_SPEED_SCALE);
