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
#include <Mocks/LightsMock.h>
#include <Mocks/StepperMotorMock.h>
#include <Mocks/SerialPortMock.h>
#include <Mocks/ServoMock.h>
#include <Mocks/StepperMotorStub.h>
#include <Mocks/FileHandlingMock.h>

#include <QtDebug>

using ::testing::ReturnRef;
using ::testing::Return;
using ::testing::_;

class GamepadTest : Gamepad
{
public:
    GamepadTest(StepperMotorShell *_stepperMotor, Servo *_servo, Lights *_lights, FileHandling *_fileHandling) : Gamepad(_stepperMotor, _servo, _lights, _fileHandling)
    {
    }

    void handleTestedButton(ButtonName ButtonName)
    {
        this->isReadGamepadInputThreadActive = true;
        ButtonEvent event;
        event.buttonName = ButtonName;
        event.buttonValue = gamepad->isButtonPressed(ButtonName);
        this->buttonEvents.push_back(event);
        this->handleButtonInput();
    }

    void handleAxisInput(AxisName axisID)
    {
        gamepad->isReadGamepadInputThreadActive = true;
        AxisEvent event;
        event.axisName = axisID;
        event.axisValue = gamepad->getAxisValue(axisID);
        gamepad->axisEvents.push_back(event);
        gamepad->handleAxisInput();
    }

    void setButton(ButtonName ButtonName, bool state)
    {
        this->buttonState[ButtonName] = state;
    }

    void setAxis(AxisName axisId, S16 value)
    {
        gamepad->axisState[axisId] = value;
    }

    void startThread()
    {
        gamepad->startThread();
    }

    void stopThread()
    {
        gamepad->stopThread();
    }

private:
        Gamepad *gamepad;
};

TEST(GamepadTest, Brake)
{
    StepperMotorShellMock *stepperMotor = new StepperMotorShellMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));

    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
    lights->setReverseLight(REVERSE_LIGHT_OFF);
    EXPECT_EQ(lights->getBrakeLightsStatus(), BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights->getReverseLightStatus(), REVERSE_LIGHT_OFF);

    const ButtonName testedButton = BRAKE_BUTTON;
    bool buttonState = BUTTON_DOWN;
    BrakeLightsCommand expectedState = BRAKE_LIGHT_STOP;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleTestedButton(testedButton);
    EXPECT_CALL(*stepperMotor, brake()).Times(1);

    lights->setReverseLight(REVERSE_LIGHT_ON);
    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.handleTestedButton(testedButton);
    EXPECT_EQ(lights->getReverseLightStatus(), expectedState);

//    buttonState = BUTTON_UP;
//    expectedState = BRAKE_LIGHT_OFF;
//    gamepadTest.setButton(testedButton, buttonState);
//    EXPECT_CALL(*stepperMotor, switchOff()).Times(1);
//    gamepadTest.handleTestedButton(testedButton);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

#ifdef DUPA
TEST(GamepadTest, LeftTurnSignal)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    ButtonName testedButton = LEFT_BLINKER_BUTTON;
    bool buttonState = BUTTON_DOWN;
    TurnSignalCommand expectedState = TURN_SIGNAL_LEFT;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = LEFT_BLINKER_BUTTON;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, RightTurnSignal)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    ButtonName testedButton = RIGHT_BLINKER_BUTTON;
    bool buttonState = BUTTON_DOWN;
    TurnSignalCommand expectedState = TURN_SIGNAL_RIGHT;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = RIGHT_BLINKER_BUTTON;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, HazardLights)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    /*
     * First pressed button Right bumper
     * Second pressed button Left bumber
     * Setting off with Right bumper
    */
    ButtonName testedButton = RIGHT_BLINKER_BUTTON;
    bool buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = LEFT_BLINKER_BUTTON;
    gamepadTest.setButton(testedButton, buttonState);
    TurnSignalCommand expectedState = HAZARD_LIGHTS;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = LEFT_BLINKER_BUTTON;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = RIGHT_BLINKER_BUTTON;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    /*
     * First pressed button Left bumper
     * Second pressed button Right bumber
     * Setting off with Left bumper
    */
    testedButton = LEFT_BLINKER_BUTTON;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = RIGHT_BLINKER_BUTTON;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = HAZARD_LIGHTS;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = RIGHT_BLINKER_BUTTON;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = LEFT_BLINKER_BUTTON;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, Headlights)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    HeadLightCommand expectedState = HEADLIGHT_OFF;
    S16 AxisValue = JoystickEvent::MIN_AXES_VALUE;
    AxisName testedAxis = ARROW_Y_AXIS;
    gamepadTest.setAxis(testedAxis, AxisValue);
    lights->setHeadLight(expectedState);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    expectedState = HEADLIGHT_DAYTIME;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    expectedState = HEADLIGHT_HIGH_BEAM;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    expectedState = HEADLIGHT_OFF;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, BrakeLights)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, lights, (FileHandling*)fileHandling);

    BrakeLightsCommand expectedState = BRAKE_LIGHT_OFF;
    S16 AxisValue = JoystickEvent::MAX_AXES_VALUE;
    AxisName testedAxis = ARROW_Y_AXIS;
    gamepadTest.setAxis(testedAxis, AxisValue);
    lights->setBrakeLightsWhenOff(expectedState);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    expectedState = BRAKE_LIGHT_DAYTIME;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, Turn)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    LightsMock *lights = new LightsMock();
    FileHandlingMock *fileHandling = new FileHandlingMock();

    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Servo *servo = new Servo((SerialPort*)serialPort, (StepperMotorShell*)stepperMotor);

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, servo, (Lights*)lights, (FileHandling*)fileHandling);

    AxisName testedAxis = LEFT_X_AXIS;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    S8 expectedAngle = Servo::MAX_ANGLE;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(servo->getAngle(), expectedAngle);

    axisValue = JoystickEvent::MIN_AXES_VALUE;
    expectedAngle = Servo::MIN_ANGLE;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(servo->getAngle(), expectedAngle);

    axisValue = 0;
    expectedAngle = 0;
    gamepadTest.setAxis(testedAxis, axisValue);
    U8 speed = 0;
    EXPECT_CALL(*stepperMotor, getSpeed).WillOnce(ReturnRef(speed));
    gamepadTest.handleAxisInput(testedAxis);
    usleep(500000);
    EXPECT_EQ(servo->getAngle(), expectedAngle);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, ForwardMovement)
{
    StepperMotorStub *stepperMotor = new StepperMotorStub();
    SerialPortMock *serialPort = new SerialPortMock();
    LightsMock *lights = new LightsMock();
    ServoMock *servo = new ServoMock;
    FileHandlingMock *fileHandling = new FileHandlingMock();

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, (Lights*)lights, (FileHandling*)fileHandling);

    //Max spped
    AxisName testedAxis = RIGHT_TRIGGER;
    StepperMotorDirection expectedDirection = DIRECTION_FORWARD;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    U8 expectedSpeed = StepperMotor::MAX_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Half spped
    axisValue = 0;
    expectedSpeed = StepperMotor::MAX_SPEED/2;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Off
    axisValue = JoystickEvent::MIN_AXES_VALUE;
    expectedSpeed = StepperMotor::MIN_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, BackwardMovement)
{
    StepperMotorStub *stepperMotor = new StepperMotorStub();
    SerialPortMock *serialPort = new SerialPortMock();
    LightsMock *lights = new LightsMock();
    ServoMock *servo = new ServoMock;
    FileHandlingMock *fileHandling = new FileHandlingMock();

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, (Lights*)lights, (FileHandling*)fileHandling);

    //Max spped
    ReverseLightCommand reverseLightStatus = REVERSE_LIGHT_OFF;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillOnce(ReturnRef(reverseLightStatus));
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    AxisName testedAxis = LEFT_TRIGGER;
    StepperMotorDirection expectedDirection = DIRECTION_BACKWARD;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    U8 expectedSpeed = StepperMotor::MAX_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Half spped
    reverseLightStatus = REVERSE_LIGHT_ON;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillOnce(ReturnRef(reverseLightStatus));
    axisValue = 0;
    expectedSpeed = StepperMotor::MAX_SPEED/2;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Off
    reverseLightStatus = REVERSE_LIGHT_ON;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillOnce(ReturnRef(reverseLightStatus));
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    axisValue = JoystickEvent::MIN_AXES_VALUE;
    expectedSpeed = StepperMotor::MIN_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, ButtonCombinations)
{
    const U16 WAIT_SHORT = 500;

    StepperMotorStub *stepperMotor = new StepperMotorStub();
    SerialPortMock *serialPort = new SerialPortMock();
    LightsMock *lights = new LightsMock();
    ServoMock *servo = new ServoMock;
    FileHandlingMock *fileHandling = new FileHandlingMock();

    const U8 brakeButtonId = 1;
    const U8 leftBlinkerButtonId = 2;
    const U8 lightBlinkerButtonId = 3;
    const U8 exitButtonId = 4;
    EXPECT_CALL(*fileHandling, getBrakeButtonId).WillOnce(Return(brakeButtonId));
    EXPECT_CALL(*fileHandling, getLeftBlinkerButtonId).WillOnce(Return(leftBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getRightBlinkerButtonId).WillOnce(Return(lightBlinkerButtonId));
    EXPECT_CALL(*fileHandling, getExitButtonId).WillOnce(Return(exitButtonId));
    GamepadTest gamepadTest((StepperMotorShell*)stepperMotor, (Servo*)servo, (Lights*)lights, (FileHandling*)fileHandling);
    stepperMotor->move(DIRECTION_FORWARD, StepperMotor::MAX_SPEED);
    gamepadTest.startThread();

    U8 expectedSpeed;
    StepperMotorDirection expectedDirection;

    //Reverse Off
    ReverseLightCommand reverseLightStatus = REVERSE_LIGHT_OFF;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillRepeatedly(ReturnRef(reverseLightStatus));

    ////////////////////////////////////////////////////
    //  @Description:
    //  Test combination of two triggers pressed at once,
    //  then release right trigger
    //
    //  @Trigger:
    //  1. Both triggers pressed, right pressed first
    //
    //  @Verification:
    //  2. Speed == 0
    //
    //  @Trigger:
    //  3. Release right trigger
    //
    //  @Verification:
    //  4. Speed == MAX_SPEED
    //  5. Direction == BACKWARD
    ////////////////////////////////////////////////////

    //  @Trigger:
    //  1. Both triggers pressed, right pressed first
    AxisName testedAxis = RIGHT_TRIGGER;
    S16 RightAxisValue = JoystickEvent::MAX_AXES_VALUE;
    S16 LeftAxisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.setAxis(LEFT_TRIGGER, LeftAxisValue);
    gamepadTest.setAxis(RIGHT_TRIGGER, RightAxisValue);
    gamepadTest.handleAxisInput(testedAxis);

    //  @Verification:
    //  2. Speed == 0
    expectedSpeed = 0;
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //  @Trigger:
    //  3. Release right trigger
    RightAxisValue = JoystickEvent::MIN_AXES_VALUE;
    gamepadTest.setAxis(testedAxis, RightAxisValue);
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    gamepadTest.handleAxisInput(testedAxis);

    //  @Verification:
    //  4. Speed > 0
    //  5. Direction == BACKWARD
    expectedSpeed = StepperMotor::MAX_SPEED;
    expectedDirection = DIRECTION_BACKWARD;
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //////////////////////////////////////////////////
    //  @Description:
    //  Test combination of two triggers pressed at once,
    //  then release left trigger
    //
    //  @Trigger:
    //  1. Both triggers pressed, left pressed first
    //
    //  @Verification:
    //  2. Speed == 0
    //
    //  @Trigger:
    //  3. Release left trigger
    //
    //  @Verification:
    //  4. Speed == MAX_SPEED
    //  5. Direction == FOWARD
    //////////////////////////////////////////////////

    //  @Trigger:
    //  1. Both triggers pressed, right pressed first
    testedAxis = LEFT_TRIGGER;
    RightAxisValue = JoystickEvent::MAX_AXES_VALUE;
    LeftAxisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.setAxis(LEFT_TRIGGER, LeftAxisValue);
    gamepadTest.setAxis(RIGHT_TRIGGER, RightAxisValue);
    gamepadTest.handleAxisInput(testedAxis);

    //  @Verification:
    //  2. Speed == 0
    expectedSpeed = 0;
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //  @Trigger:
    //  3. Release right trigger
    LeftAxisValue = JoystickEvent::MIN_AXES_VALUE;
    gamepadTest.setAxis(testedAxis, LeftAxisValue);
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    gamepadTest.handleAxisInput(testedAxis);

    //  @Verification:
    //  4. Speed > 0
    //  5. Direction == BACKWARD
    expectedSpeed = StepperMotor::MAX_SPEED;
    expectedDirection = DIRECTION_FORWARD;
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //////////////////////////////////////////////////
    //  @Description:
    //  Test combination of right trigger and brake
    //  pressed, then release brake
    //
    //  @Trigger:
    //  1. Right trigger and brake pressed.
    //
    //  @Verification:
    //  2. Speed == 0
    //
    //  @Trigger:
    //  3. Release brake
    //
    //  @Verification:
    //  4. Speed == MAX_SPEED
    //  5. Direction == FORWARD
    //////////////////////////////////////////////////

    //  @Trigger:
    //  1. Right trigger and brake pressed.
    ButtonName brakeButton = BRAKE_BUTTON;
    bool buttonValue = BUTTON_DOWN;
    testedAxis = LEFT_TRIGGER;
    RightAxisValue = JoystickEvent::MAX_AXES_VALUE;
    LeftAxisValue = JoystickEvent::MIN_AXES_VALUE;
    gamepadTest.setAxis(LEFT_TRIGGER, LeftAxisValue);
    gamepadTest.setAxis(RIGHT_TRIGGER, RightAxisValue);
    gamepadTest.setButton(brakeButton, buttonValue);
    EXPECT_CALL(*lights, setBrakeLights(_)).Times(1);
    gamepadTest.handleButtonInput(brakeButton);

    //  @Verification:
    //  2. Speed == 0
    expectedSpeed = 0;
    usleep(WAIT_SHORT);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //  @Trigger:
    //  3. Release brake
    buttonValue = BUTTON_UP;
    gamepadTest.setButton(brakeButton, buttonValue);
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    EXPECT_CALL(*lights, setBrakeLights(_)).Times(1);
    gamepadTest.handleButtonInput(brakeButton);

    //  @Verification:
    //  4. Speed > 0
    //  5. Direction == FORWARD
    expectedDirection = DIRECTION_FORWARD;
    expectedSpeed = StepperMotor::MAX_SPEED;
    usleep(WAIT_SHORT);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //////////////////////////////////////////////////
    //  @Description:
    //  Test combination of left trigger and brake
    //  pressed, then release brake
    //
    //  @Trigger:
    //  1. Left trigger and brake pressed.
    //
    //  @Verification:
    //  2. Speed == 0
    //
    //  @Trigger:
    //  3. Release brake
    //
    //  @Verification:
    //  4. Speed == MAX_SPEED
    //  5. Direction == BACKWARD
    //////////////////////////////////////////////////

    //  @Trigger:
    //  1. Left trigger and brake pressed.
    brakeButton = BRAKE_BUTTON;
    buttonValue = BUTTON_DOWN;
    testedAxis = LEFT_TRIGGER;
    RightAxisValue = JoystickEvent::MIN_AXES_VALUE;
    LeftAxisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.setAxis(LEFT_TRIGGER, LeftAxisValue);
    gamepadTest.setAxis(RIGHT_TRIGGER, RightAxisValue);
    gamepadTest.setButton(brakeButton, buttonValue);
    EXPECT_CALL(*lights, setBrakeLights(_)).Times(1);
    gamepadTest.handleButtonInput(brakeButton);

    //  @Verification:
    //  2. Speed == 0
    expectedSpeed = 0;
    usleep(WAIT_SHORT);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //  @Trigger:
    //  3. Release brake
    buttonValue = BUTTON_UP;
    gamepadTest.setButton(brakeButton, buttonValue);
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    EXPECT_CALL(*lights, setBrakeLights(_)).Times(1);
    gamepadTest.handleButtonInput(brakeButton);

    //  @Verification:
    //  4. Speed > 0
    //  5. Direction == BACKWARD
    expectedDirection = DIRECTION_BACKWARD;
    expectedSpeed = StepperMotor::MAX_SPEED;
    usleep(WAIT_SHORT);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    gamepadTest.stopThread();
    delete fileHandling;
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}
#endif
