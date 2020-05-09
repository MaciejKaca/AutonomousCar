#pragma once

#include <inc/common.h>
#include <inc/steppermotor.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <joystick/joystick.hh>

#include <unistd.h>
#include <map>
#include <future>
#include <vector>

#include "Base/GamepadBase.h"

class Gamepad : GamepadBase
{
    friend class GamepadTest;

    public:
        Gamepad(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights);
        void startThread();
        void stopThread();
        void waitForExitButton();
        void readGamepadInput() override;
        ~Gamepad();

    private:
        struct ButtonEvent
        {
            ButtonID buttonID;
            bool buttonValue;
        };

        struct AxisEvent
        {
            AxisID axisID;
            short axisValue;
        };

        const std::string gamepadPath = "/dev/input/js0";
        const float AXIS_TO_SPEED_SCALE;
        const float AXIS_TO_ANGLE_SCALE;
        const S16 AXIS_MIN_TRIGGER_POSITION;
        const U8 GAMEPAD_REFRESH_TIME = 1;
        const U8 NUMBER_OF_BUTTONS = 10;
        const U8 NUMBER_OF_AXIS = 7;

        Joystick *joystick;
        StepperMotor *stepperMotor;
        Servo *servo;
        Lights *lights;

        bool isReadGamepadInputThreadActive;
        bool isHandleButtonInputsThreadActive;
        bool isHandleAxisInputsThreadActive;

        std::future<void> readGamepadInputThread;
        std::future<void> handleButtonInputsThread;
        std::future<void> handleAxisInputsThread;

        void clearInput();
        bool isGamepadConnected();
        U16 axisToSpeed(const S16 &axisValue);
        S8 axisToDegrees(const S16 &axisValue);
        std::map<ButtonID, bool> buttonState;
        std::map<AxisID, short> axisState;
        std::vector<ButtonEvent> buttonEvents;
        std::vector<AxisEvent> axisEvents;

        void handleButtonInput();
        void handleAxisInput();
        void launchHandleButtonInputThread();
        void launchHandleAxisInputThread();

        const bool &isButtonPressed(const ButtonID &buttonID);
        const short &getAxisValue(const AxisID &axisID);
};
