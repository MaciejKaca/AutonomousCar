#pragma once

#include <joystick/joystick.hh>
#include <inc/steppermotor.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/serialport.h>
#include <inc/gamepad.h>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>
#include <Mocks/JoystickMock.h>

using ::testing::Return;
using ::testing::_;

class GamepadTest : Gamepad
{
public:
    GamepadTest(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights, JoystickMock *_joystick):Gamepad(_stepperMotor,_servo,_lights,_joystick)
    {
        gamepad = new Gamepad(_stepperMotor, _servo, _lights, _joystick);
    }

    void handleInput(const ButtonID buttonID)
    {
        gamepad->handleInput(buttonID);
    }

private:
        Gamepad *gamepad;
};

TEST(GamepadTest, clearInput)
{
    SerialPort *serialPort =(SerialPort*)malloc(sizeof(SerialPort));
    Servo *servo = (Servo*)malloc(sizeof(Servo));
    Lights *lights = new Lights(serialPort);
    StepperMotor *stepperMotor = (StepperMotor*)malloc(sizeof(StepperMotor));
    JoystickMock *joystickMock = new JoystickMock();

    GamepadTest gamepadTest(stepperMotor, servo, lights, joystickMock);

    gamepadTest.handleInput(X_BUTTON);

    ASSERT_TRUE(lights->getBrakeLightsStatus() == BRAKE_LIGHT_OFF);

    delete serialPort;
    delete servo;
    delete lights;
    delete stepperMotor;
    delete joystickMock;
}
