#pragma once

#include <CarInterface/interface.h>
#include <QDebug>

#include <cstdint>
#include <cstdlib>
#include <cmath>

#include <bits/stdc++.h>

typedef uint32_t U32;
typedef int32_t S32;
typedef uint16_t U16;
typedef int16_t S16;
typedef uint8_t U8;
typedef int8_t S8;

enum ExitReason
{
    EXIT_BY_BUTTON = 0,
    EXIT_BY_MISSING_GAMEPAD = 1,
    EXIT_BY_FAILED_GPIO = 2,
    EXIT_BY_MISSING_MODULE = 3,
    EXIT_BY_MISSING_DISTANCE_SENSOR = 4
};

const S8 LIGHTS_AND_SERVO_PORT_BASE[] = "ttyUSB0";
const S8 LIGHTS_AND_SERVO_PORT_BACKUP[] = "ttyUSB1";

const bool BUTTON_DOWN = true;
const bool BUTTON_UP = false;

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

enum ButtonID
{
    EXIT_BUTTON = 7,
    X_BUTTON = 2,
    A_BUTTON = 0,
    RIGHT_BUMPER = 5,
    LEFT_BUMPER = 4
    };

enum AxisID
{
    LEFT_X_AXIS = 0,
    RIGHT_TRIGGER = 4,
    LEFT_TRIGGER = 5,
    ARROW_Y_AXIS = 7
};

enum SensorAlignment
{
    SENSOR_FRONT = 0,
    SENSOR_FRONT_LEFT = 1,
    SENSOR_FRONT_RIGHT = 2,
    SENSOR_REAR = 3,
    SENSOR_REAR_LEFT = 4,
    SENSOR_REAR_RIGHT = 5,
    SENSOR_LEFT_SIDE = 6,
    SENSOR_RIGHT_SIDE = 7
};
