#pragma once

#include <inc/common.h>
#include <inc/steppermotor.h>
#include <inc/servo.h>
#include <inc/lights.h>

#include <joystick/joystick.hh>
#include <unistd.h>
#include <map>

class Gamepad
{
    friend class GamepadTest;

    public:
        Gamepad(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights);
        Gamepad(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights, Joystick *_joystick);
        void readGamepadInput();

    private:
        Joystick *joystick;
        StepperMotor *stepperMotor;
        Servo *servo;
        Lights *lights;

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
