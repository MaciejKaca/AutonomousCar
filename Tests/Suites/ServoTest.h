#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>

#include <CarInterface/interface.h>
#include <Mocks/SerialPortMock.h>
#include <inc/common.h>
#include <inc/servo.h>

using ::testing::Return;
using ::testing::_;

TEST(ServoTest, turn)
{
    SerialPortMock *serialPort = new SerialPortMock();
    EXPECT_CALL(*serialPort, send(_, _)).WillOnce(Return(true));
    Servo servo((SerialPort*)serialPort);
    EXPECT_EQ(servo.getAngle(), 0);

    //Right
    S8 expectedAngle = Servo::MAX_ANGLE;
    EXPECT_CALL(*serialPort, send(_, _)).WillOnce(Return(true));
    servo.turn(expectedAngle);
    EXPECT_EQ(servo.getAngle(), expectedAngle);

    //Left
    expectedAngle = Servo::MIN_ANGLE;
    EXPECT_CALL(*serialPort, send(_, _)).WillOnce(Return(true));
    servo.turn(expectedAngle);
    EXPECT_EQ(servo.getAngle(), expectedAngle);

    //Test angle validation
    expectedAngle = Servo::MIN_ANGLE;
    S8 invalidAngle = Servo::MIN_ANGLE - 1;
    servo.turn(invalidAngle);
    EXPECT_EQ(servo.getAngle(), expectedAngle);

    //Test angle validation
    expectedAngle = Servo::MIN_ANGLE;
    invalidAngle = Servo::MAX_ANGLE + 1;
    servo.turn(invalidAngle);
    EXPECT_EQ(servo.getAngle(), expectedAngle);

    delete serialPort;
}
