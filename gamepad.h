#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "common.h"
#include "steppermotor.h"
#include "serialport.h"

#include <joystick/joystick.hh>
#include <unistd.h>
#include <math.h>
#include <map>

class Gamepad
{
    public:
        Gamepad(StepperMotor *  _stepperMotor, SerialPort * _lightsAndServoSerial);
        void readGamepadInput();

    private:
        Joystick joystick;
        void clearInput();
        bool isGamepadConnected();
        uint16_t axisToSpeed(int16_t axisValue);
        const std::string gamepadPath = "/dev/input/js0";
        StepperMotor * stepperMotor;
        bool isBrakePressed;
        bool isLeftTriggerPressed;
        bool isRightTriggerPressed;
        SerialPort * lightsAndServoSerial;
        int8_t axisToDegrees(int16_t axisValue);
        std::map<ButtonID, bool> buttonState;
        std::map<AxisID, short> axisState;
        void handleInput(ButtonID buttonID);
        void handleInput(AxisID axisID);
        bool isButtonPressed(ButtonID buttonID);
        short getAxisValue(AxisID axisID);
};

#endif // GAMEPAD_H
