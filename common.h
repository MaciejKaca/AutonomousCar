#ifndef BUTTONS_H
#define BUTTONS_H

#include <cstdint>

const uint8_t GAMEPAD_REFRESH_TIME = 1; //Time in microseconds

const uint8_t EXIT_BY_BUTTON = 1;
const uint8_t EXIT_BY_MISSING_GAMEPAD = 2;
const uint8_t EXIT_BY_FAILED_GPIO = 3;

const bool BUTTON_DOWN = true;
const bool BUTTON_UP = false;

const uint8_t NUMBER_OF_BUTTONS = 10;
const uint8_t NUMBER_OF_AXIS = 7;

const uint8_t XBOX_BUTTON = 8;

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

const uint16_t MAX_SPEED = 1000;
const uint16_t MIN_SPEED = 300;

const uint8_t STEPPER_ENABLE_PIN = 16;
const uint8_t STEPPER_STEP_PIN = 21;
const uint8_t STEPPER_DIRECTION_PIN = 20;

#endif // BUTTONS_H
