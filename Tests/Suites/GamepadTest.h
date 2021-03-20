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

#include <QtDebug>

using ::testing::ReturnRef;
using ::testing::Return;
using ::testing::_;

class GamepadTest : Gamepad
{
public:
    GamepadTest(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights):Gamepad(_stepperMotor,_servo,_lights)
    {
        gamepad = new Gamepad(_stepperMotor, _servo, _lights);
    }

    void handleButtonInput(ButtonID buttonID)
    {
        gamepad->isReadGamepadInputThreadActive = true;
        ButtonEvent event;
        event.buttonID = buttonID;
        event.buttonValue = gamepad->isButtonPressed(buttonID);
        gamepad->buttonEvents.push_back(event);
        gamepad->handleButtonInput();
    }

    void handleAxisInput(AxisID axisID)
    {
        gamepad->isReadGamepadInputThreadActive = true;
        AxisEvent event;
        event.axisID = axisID;
        event.axisValue = gamepad->getAxisValue(axisID);
        gamepad->axisEvents.push_back(event);
        gamepad->handleAxisInput();
    }

    void setButton(ButtonID buttonId, bool state)
    {
        gamepad->buttonState[buttonId] = state;
    }

    void setAxis(AxisID axisId, S16 value)
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

    ~GamepadTest()
    {
        delete gamepad;
    }

private:
        Gamepad *gamepad;
};


TEST(GamepadTest, Brake)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);
    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
    ASSERT_TRUE(lights->getBrakeLightsStatus() == BRAKE_LIGHT_OFF);

    ButtonID testedButton = X_BUTTON;
    bool buttonState = BUTTON_DOWN;
    BrakeLightsCommand expectedState = BRAKE_LIGHT_STOP;
    gamepadTest.setButton(testedButton, buttonState);
    EXPECT_CALL(*stepperMotor, brake()).Times(1);
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getBrakeLightsStatus() == expectedState);

    testedButton = X_BUTTON;
    buttonState = BUTTON_UP;
    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    EXPECT_CALL(*stepperMotor, switchOff()).Times(1);
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getBrakeLightsStatus() == expectedState);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, LeftTurnSignal)
{
    StepperMotorMock *stepperMotor = new StepperMotorMock();
    SerialPortMock *serialPort = new SerialPortMock();
    ServoMock *servo = new ServoMock();
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);

    ButtonID testedButton = LEFT_BUMPER;
    bool buttonState = BUTTON_DOWN;
    TurnSignalCommand expectedState = TURN_SIGNAL_LEFT;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);

    ButtonID testedButton = RIGHT_BUMPER;
    bool buttonState = BUTTON_DOWN;
    TurnSignalCommand expectedState = TURN_SIGNAL_RIGHT;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = RIGHT_BUMPER;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleButtonInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);

    /*
     * First pressed button Right bumper
     * Second pressed button Left bumber
     * Setting off with Right bumper
    */
    ButtonID testedButton = RIGHT_BUMPER;
    bool buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = LEFT_BUMPER;
    gamepadTest.setButton(testedButton, buttonState);
    TurnSignalCommand expectedState = HAZARD_LIGHTS;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = RIGHT_BUMPER;
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
    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = RIGHT_BUMPER;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = HAZARD_LIGHTS;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = RIGHT_BUMPER;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.handleButtonInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);

    HeadLightCommand expectedState = HEADLIGHT_OFF;
    S16 AxisValue = JoystickEvent::MIN_AXES_VALUE;
    AxisID testedAxis = ARROW_Y_AXIS;
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

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Lights *lights = new Lights((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, lights);

    BrakeLightsCommand expectedState = BRAKE_LIGHT_OFF;
    S16 AxisValue = JoystickEvent::MAX_AXES_VALUE;
    AxisID testedAxis = ARROW_Y_AXIS;
    gamepadTest.setAxis(testedAxis, AxisValue);
    lights->setBrakeLightsWhenOff(expectedState);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    expectedState = BRAKE_LIGHT_DAYTIME;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.handleAxisInput(testedAxis);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    EXPECT_CALL(*serialPort, send(_, _)).WillRepeatedly(Return());
    Servo *servo = new Servo((SerialPort*)serialPort, (StepperMotor*)stepperMotor);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, servo, (Lights*)lights);

    AxisID testedAxis = LEFT_X_AXIS;
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

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
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
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, (Lights*)lights);

    //Max spped
    AxisID testedAxis = RIGHT_TRIGGER;
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
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, (Lights*)lights);

    //Max spped
    ReverseLightCommand reverseLightStatus = REVERSE_LIGHT_OFF;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillOnce(ReturnRef(reverseLightStatus));
    EXPECT_CALL(*lights, setReverseLight(_)).Times(1);
    AxisID testedAxis = LEFT_TRIGGER;
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
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, (Lights*)lights);
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
    AxisID testedAxis = RIGHT_TRIGGER;
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
    ButtonID brakeButton = X_BUTTON;
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
    brakeButton = X_BUTTON;
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
    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}
