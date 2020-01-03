#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "common.h"
#include "steppermotor.h"
#include "serialport.h"
#include "servo.h"

#include <joystick/joystick.hh>
#include <unistd.h>
#include <math.h>
#include <map>

class Gamepad
{
    public:
        Gamepad(StepperMotor *  _stepperMotor, Servo * _servo);
        void readGamepadInput();

    private:
        Joystick joystick;
        StepperMotor * stepperMotor;
        Servo * servo;

        void clearInput();
        bool isGamepadConnected();
        const std::string gamepadPath = "/dev/input/js0";

        U16 axisToSpeed(S16 axisValue);
        S8 axisToDegrees(S16 axisValue);

        std::map<ButtonID, bool> buttonState;
        std::map<AxisID, short> axisState;

        void handleInput(ButtonID buttonID);
        void handleInput(AxisID axisID);

        bool isButtonPressed(ButtonID buttonID);

        short getAxisValue(AxisID axisID);
};

#endif // GAMEPAD_H
