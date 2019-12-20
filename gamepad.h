#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "common.h"
#include "steppermotor.h"

#include <joystick/joystick.hh>
#include <unistd.h>
#include <math.h>

class Gamepad
{
public:
    Gamepad();
    void handleInput();

private:
    Joystick joystick;
    void clearInput();
    bool isGamepadConnected();
    uint16_t axisToSpeed(int16_t axisValue);
    const std::string gamepadPath = "/dev/input/js0";
    StepperMotor stepperMotor;
};

#endif // GAMEPAD_H
