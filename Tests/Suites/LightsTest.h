#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>

#include <CarInterface/interface.h>
#include <Mocks/SerialPortMock.h>
#include <inc/common.h>
#include <inc/lights.h>

using ::testing::ReturnRef;
using ::testing::Return;
using ::testing::_;

TEST(LightsTest, set)
{
    qInfo("LightsTest, set\n");
    SerialPortMock *serialPort = new SerialPortMock();

    EXPECT_CALL(*serialPort, send(_, _)).Times(6);
    Lights *lights = new Lights((SerialPort*)serialPort);
    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
    lights->setHeadLight(HEADLIGHT_OFF);
    EXPECT_EQ(lights->getTurnSignalStatus(), TURN_SIGNAL_OFF);
    EXPECT_EQ(lights->getHeadLightStatus(), HEADLIGHT_OFF);
    EXPECT_EQ(lights->getBrakeLightsStatus(), BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights->getReverseLightStatus(), REVERSE_LIGHT_OFF);

    TurnSignalCommand expectedTurnSignalCommand = TURN_SIGNAL_LEFT;
    EXPECT_CALL(*serialPort, send(_, _)).Times(1);
    lights->setTurnSignal(expectedTurnSignalCommand);
    EXPECT_EQ(lights->getTurnSignalStatus(),expectedTurnSignalCommand);

    HeadLightCommand expectedHeadLightCommand = HEADLIGHT_DAYTIME;
    EXPECT_CALL(*serialPort, send(_, _)).Times(1);
    lights->setHeadLight(expectedHeadLightCommand);
    EXPECT_EQ(lights->getHeadLightStatus(), HEADLIGHT_DAYTIME);

    BrakeLightsCommand expectedBrakeLightsCommand = BRAKE_LIGHT_STOP;
    EXPECT_CALL(*serialPort, send(_, _)).Times(1);
    lights->setBrakeLights(expectedBrakeLightsCommand);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedBrakeLightsCommand);

    ReverseLightCommand expectedReverseLightCommand = REVERSE_LIGHT_ON;
    EXPECT_CALL(*serialPort, send(_, _)).Times(1);
    lights->setReverseLight(expectedReverseLightCommand);
    EXPECT_EQ(lights->getReverseLightStatus(), expectedReverseLightCommand);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
    delete lights;
    delete serialPort;
}

TEST(LightsTest, defaultBrakeLightOff)
{
    qInfo("LightsTest, defaultBrakeLightOff\n");
    SerialPortMock *serialPort = new SerialPortMock();

    EXPECT_CALL(*serialPort, send(_, _)).Times(5);
    Lights *lights = new Lights((SerialPort*)serialPort);
    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights->getBrakeLightsWhenOffStatus(), BRAKE_LIGHT_OFF);

    EXPECT_CALL(*serialPort, send(_, _)).Times(1);
    BrakeLightsCommand expectedBrakeLightsCommand = BRAKE_LIGHT_DAYTIME;
    lights->setBrakeLightsWhenOff(expectedBrakeLightsCommand);
    EXPECT_EQ(lights->getBrakeLightsWhenOffStatus(), expectedBrakeLightsCommand);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedBrakeLightsCommand);

    EXPECT_CALL(*serialPort, send(_, _)).Times(2);
    lights->setBrakeLights(BRAKE_LIGHT_STOP);
    lights->setBrakeLights(BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedBrakeLightsCommand);

    EXPECT_CALL(*serialPort, send(_, _)).Times(3);
    expectedBrakeLightsCommand = BRAKE_LIGHT_OFF;
    lights->setBrakeLightsWhenOff(expectedBrakeLightsCommand);
    lights->setBrakeLights(BRAKE_LIGHT_STOP);
    lights->setBrakeLights(BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights->getBrakeLightsStatus(), expectedBrakeLightsCommand);

    bool serialState = false;
    EXPECT_CALL(*serialPort, isSerialOpen).WillOnce(ReturnRef(serialState));
    delete lights;
    delete serialPort;
}
