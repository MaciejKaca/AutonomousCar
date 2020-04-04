#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>

#include <inc/common.h>
#include <inc/steppermotor.h>

TEST(StepperMotorTest, moveMinSpeed)
{
    StepperMotor stepperMotor;
    EXPECT_EQ(stepperMotor.getSpeed(), 0);
    ASSERT_FALSE(stepperMotor.isThreadActive());

    U16 expectedSpeed = StepperMotor::MIN_SPEED+1;
    StepperMotorDirection expectedDirection = FORWARD;
    stepperMotor.move(expectedDirection, expectedSpeed);
    ASSERT_TRUE(stepperMotor.isThreadActive());
    usleep(500000);
    EXPECT_EQ(stepperMotor.getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor.getDirection(), expectedDirection);
    stepperMotor.switchOff();
}

TEST(StepperMotorTest, moveMaxSpeed)
{
    StepperMotor stepperMotor;
    EXPECT_EQ(stepperMotor.getSpeed(), 0);
    ASSERT_FALSE(stepperMotor.isThreadActive());

    U16 expectedSpeed = StepperMotor::MAX_SPEED;
    StepperMotorDirection expectedDirection = BACKWARD;
    stepperMotor.move(expectedDirection, expectedSpeed);
    ASSERT_TRUE(stepperMotor.isThreadActive());
    sleep(5);
    EXPECT_EQ(stepperMotor.getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor.getDirection(), expectedDirection);
    stepperMotor.switchOff();
}

TEST(StepperMotorTest, deceleration)
{
    StepperMotor stepperMotor;
    EXPECT_EQ(stepperMotor.getSpeed(), 0);
    ASSERT_FALSE(stepperMotor.isThreadActive());

    U16 expectedSpeed = StepperMotor::MAX_SPEED;
    StepperMotorDirection expectedDirection = BACKWARD;
    stepperMotor.move(expectedDirection, expectedSpeed);
    ASSERT_TRUE(stepperMotor.isThreadActive());
    sleep(5);
    EXPECT_EQ(stepperMotor.getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor.getDirection(), expectedDirection);

    expectedSpeed = StepperMotor::MIN_SPEED+1;
    stepperMotor.move(expectedDirection, expectedSpeed);
    ASSERT_TRUE(stepperMotor.isThreadActive());
    sleep(5);
    EXPECT_EQ(stepperMotor.getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor.getDirection(), expectedDirection);

    stepperMotor.switchOff();
}
