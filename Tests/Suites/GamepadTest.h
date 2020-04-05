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

    void handleInput(const ButtonID buttonID)
    {
        gamepad->handleInput(buttonID);
    }

    void handleInput(const AxisID buttonID)
    {
        gamepad->handleInput(buttonID);
    }

    void setButton(ButtonID buttonId, bool state)
    {
        gamepad->buttonState[buttonId] = state;
    }

    void setAxis(AxisID axisId, S16 value)
    {
        gamepad->axisState[axisId] = value;
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
    gamepadTest.handleInput(testedButton);
    ASSERT_TRUE(lights->getBrakeLightsStatus() == expectedState);

    testedButton = X_BUTTON;
    buttonState = BUTTON_UP;
    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    EXPECT_CALL(*stepperMotor, switchOff()).Times(1);
    gamepadTest.handleInput(testedButton);
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
    gamepadTest.handleInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleInput(testedButton);
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
    gamepadTest.handleInput(testedButton);
    EXPECT_EQ(lights->getTurnSignalStatus(), expectedState);

    testedButton = RIGHT_BUMPER;
    buttonState = BUTTON_DOWN;
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.setButton(testedButton, buttonState);
    gamepadTest.handleInput(testedButton);
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
    gamepadTest.handleInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = RIGHT_BUMPER;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.handleInput(testedButton);
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
    gamepadTest.handleInput(testedButton);
    ASSERT_TRUE(lights->getTurnSignalStatus() == expectedState);

    testedButton = RIGHT_BUMPER;
    buttonState = BUTTON_UP;
    gamepadTest.setButton(testedButton, buttonState);
    testedButton = LEFT_BUMPER;
    buttonState = BUTTON_DOWN;
    gamepadTest.setButton(testedButton, buttonState);
    expectedState = TURN_SIGNAL_OFF;
    gamepadTest.handleInput(testedButton);
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
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    expectedState = HEADLIGHT_HIGH_BEAM;
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(lights->getHeadLightStatus(), expectedState);

    expectedState = HEADLIGHT_OFF;
    gamepadTest.handleInput(testedAxis);
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
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedState);

    expectedState = BRAKE_LIGHT_OFF;
    gamepadTest.handleInput(testedAxis);
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
    Servo *servo = new Servo((SerialPort*)serialPort);
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, servo, (Lights*)lights);

    AxisID testedAxis = LEFT_X_AXIS;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    S8 expectedAngle = Servo::MAX_ANGLE;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(servo->getAngle(), expectedAngle);

    axisValue = JoystickEvent::MIN_AXES_VALUE;
    expectedAngle = Servo::MIN_ANGLE;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(servo->getAngle(), expectedAngle);

    axisValue = 0;
    expectedAngle = 0;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
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
    StepperMotorDirection expectedDirection = FORWARD;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    U8 expectedSpeed = StepperMotor::MAX_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Half spped
    axisValue = 0;
    expectedSpeed = StepperMotor::MAX_SPEED/2;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Off
    axisValue = JoystickEvent::MIN_AXES_VALUE;
    expectedSpeed = StepperMotor::MIN_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
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
    StepperMotorDirection expectedDirection = BACKWARD;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    U8 expectedSpeed = StepperMotor::MAX_SPEED;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    //Half spped
    reverseLightStatus = REVERSE_LIGHT_ON;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillOnce(ReturnRef(reverseLightStatus));
    axisValue = 0;
    expectedSpeed = StepperMotor::MAX_SPEED/2;
    gamepadTest.setAxis(testedAxis, axisValue);
    gamepadTest.handleInput(testedAxis);
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
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor->getDirection(), expectedDirection);

    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}

TEST(GamepadTest, BothTrigersAtOnce)
{
    StepperMotorStub *stepperMotor = new StepperMotorStub();
    SerialPortMock *serialPort = new SerialPortMock();
    LightsMock *lights = new LightsMock();
    ServoMock *servo = new ServoMock;
    GamepadTest gamepadTest((StepperMotor*)stepperMotor, (Servo*)servo, (Lights*)lights);
    stepperMotor->move(FORWARD, StepperMotor::MAX_SPEED);
    //Off
    ReverseLightCommand reverseLightStatus = REVERSE_LIGHT_OFF;
    EXPECT_CALL(*lights, getReverseLightStatus)
        .WillRepeatedly(ReturnRef(reverseLightStatus));

    //Right pressed first
    AxisID testedAxis = LEFT_TRIGGER;
    S16 axisValue = JoystickEvent::MAX_AXES_VALUE;
    U8 expectedSpeed = 0;
    gamepadTest.setAxis(LEFT_TRIGGER, axisValue);
    gamepadTest.setAxis(RIGHT_TRIGGER, axisValue);
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //Right pressed first
    testedAxis = RIGHT_TRIGGER;
    axisValue = JoystickEvent::MAX_AXES_VALUE;
    expectedSpeed = 0;
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //Both and brake, Left first
    testedAxis = LEFT_TRIGGER;
    axisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //Both and brake, Right first
    testedAxis = RIGHT_TRIGGER;
    axisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.handleInput(testedAxis);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    //Both and brake, button triggers action
    axisValue = JoystickEvent::MAX_AXES_VALUE;
    gamepadTest.setButton(X_BUTTON, BUTTON_DOWN);
    EXPECT_CALL(*lights, setBrakeLights(_)).Times(1);
    gamepadTest.handleInput(X_BUTTON);
    EXPECT_EQ(stepperMotor->getSpeed(), expectedSpeed);

    delete lights;
    delete servo;
    delete serialPort;
    delete stepperMotor;
}
