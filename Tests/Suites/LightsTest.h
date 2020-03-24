#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>

#include <Mocks/SerialPortMock.h>
#include <inc/lights.h>
#include <inc/common.h>

using ::testing::Return;
using ::testing::_;


TEST(LightsTest, set)
{
    SerialPortMock *serialPort = new SerialPortMock();

    EXPECT_CALL(*serialPort, send(_, _))
            .Times(4)
            .WillRepeatedly(Return(true));
    Lights lights((SerialPort*)serialPort);
    EXPECT_EQ(lights.getTurnSignalStatus(), TURN_SIGNAL_OFF);
    EXPECT_EQ(lights.getHeadLightStatus(), HEADLIGHT_OFF);
    EXPECT_EQ(lights.getBrakeLightsStatus(), BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights.getReverseLightStatus(), REVERSE_LIGHT_OFF);

    TurnSignalCommand expectedTurnSignalCommand = TURN_SIGNAL_LEFT;
    EXPECT_CALL(*serialPort, send(_, _))
            .WillOnce(Return(true));
    lights.setTurnSignal(expectedTurnSignalCommand);
    EXPECT_EQ(lights.getTurnSignalStatus(),expectedTurnSignalCommand);

    HeadLightCommand expectedHeadLightCommand = HEADLIGHT_DAYTIME;
    EXPECT_CALL(*serialPort, send(_, _))
            .WillOnce(Return(true));
    lights.setHeadLight(expectedHeadLightCommand);
    EXPECT_EQ(lights.getHeadLightStatus(), HEADLIGHT_DAYTIME);

    BrakeLightsCommand expectedBrakeLightsCommand = BRAKE_LIGHT_STOP;
    EXPECT_CALL(*serialPort, send(_, _))
            .WillOnce(Return(true));
    lights.setBrakeLights(expectedBrakeLightsCommand);
    EXPECT_EQ(lights.getBrakeLightsStatus(), expectedBrakeLightsCommand);

    ReverseLightCommand expectedReverseLightCommand = REVERSE_LIGHT_ON;
    EXPECT_CALL(*serialPort, send(_, _))
            .WillOnce(Return(true));
    lights.setReverseLight(expectedReverseLightCommand);
    EXPECT_EQ(lights.getReverseLightStatus(), expectedReverseLightCommand);

    delete serialPort;
}

TEST(LightsTest, defaultBrakeLightOff)
{
    SerialPortMock *serialPort = new SerialPortMock();

    EXPECT_CALL(*serialPort, send(_, _))
            .WillRepeatedly(Return(true));
    Lights lights((SerialPort*)serialPort);
    EXPECT_EQ(lights.getBrakeLightsWhenOffStatus(), BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights.getBrakeLightsWhenOffStatus(), BRAKE_LIGHT_OFF);

    BrakeLightsCommand expectedBrakeLightsCommand = BRAKE_LIGHT_DAYTIME;
    lights.setBrakeLightsWhenOff(expectedBrakeLightsCommand);
    EXPECT_EQ(lights.getBrakeLightsWhenOffStatus(), expectedBrakeLightsCommand);
    EXPECT_EQ(lights.getBrakeLightsStatus(), expectedBrakeLightsCommand);

    lights.setBrakeLights(BRAKE_LIGHT_STOP);
    lights.setBrakeLights(BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights.getBrakeLightsStatus(), expectedBrakeLightsCommand);

    expectedBrakeLightsCommand = BRAKE_LIGHT_OFF;
    lights.setBrakeLightsWhenOff(expectedBrakeLightsCommand);
    lights.setBrakeLights(BRAKE_LIGHT_STOP);
    lights.setBrakeLights(BRAKE_LIGHT_OFF);
    EXPECT_EQ(lights.getBrakeLightsStatus(), expectedBrakeLightsCommand);

    delete serialPort;
}
