#pragma once

#include <inc/common.h>
#include <inc/StepperMotorShell.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/FileHandling.h>
#include <joystick/joystick.hh>

#include <unistd.h>
#include <map>
#include <future>
#include <vector>

class Gamepad
{
    friend class GamepadTest;

    public:
        Gamepad(StepperMotorShell *_stepperMotor, Servo *_servo, Lights *_lights, FileHandling *_settings);
        void startThread();
        void stopThread();
        void waitForExitButton();
        ~Gamepad();

    private:
        struct ButtonEvent
        {
            ButtonName buttonName;
            bool buttonValue;
        };

        struct AxisEvent
        {
            AxisName axisName;
            short axisValue;
        };

        const std::string gamepadPath = "/dev/input/js0";
        const float AXIS_TO_SPEED_SCALE;
        const float AXIS_TO_ANGLE_SCALE;
        const S16 AXIS_MIN_TRIGGER_POSITION;
        const U8 GAMEPAD_REFRESH_TIME = 1;

        static const U8 NUMBER_OF_BUTTONS = 10;
        static const U8 NUMBER_OF_AXIS = 8;

        Joystick *joystick;
        StepperMotorShell *stepperMotor;
        Servo *servo;
        Lights *lights;
        FileHandling *settings;

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

        std::map<ButtonName, bool> buttonState;
        std::map<AxisName, short> axisState;
        std::vector<ButtonEvent> buttonEvents;
        std::vector<AxisEvent> axisEvents;
        std::array<bool, NUMBER_OF_AXIS> mustAxisReturnToZero;
        std::map<U8, ButtonName> buttonMapping;

        void handleButtonInput();
        void handleAxisInput();
        void launchHandleButtonInputThread();
        void launchHandleAxisInputThread();
        void readGamepadInput();

        const bool &isButtonPressed(const ButtonName &buttonName);
        const short &getAxisValue(const AxisName &axisName);
};
