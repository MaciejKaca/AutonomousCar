#pragma once

#include "common.h"
#include "steppermotor.h"
#include "servo.h"
#include "lights.h"

#include <joystick/joystick.hh>
#include <unistd.h>
#include <map>

class Gamepad
{
    public:
        Gamepad(StepperMotor *  _stepperMotor, Servo * _servo, Lights * _lights);
        void readGamepadInput();

    private:
        Joystick joystick;
        StepperMotor * stepperMotor;
        Servo * servo;
        Lights * lights;

        void clearInput();
        bool isGamepadConnected();
        const std::string gamepadPath = "/dev/input/js0";

        U16 axisToSpeed(const S16 axisValue);
        S8 axisToDegrees(const S16 axisValue);

        std::map<ButtonID, bool> buttonState;
        std::map<AxisID, short> axisState;

        void handleInput(const ButtonID buttonID);
        void handleInput(const AxisID axisID);

        bool isButtonPressed(const ButtonID buttonID);

        short getAxisValue(const AxisID axisID);
};
