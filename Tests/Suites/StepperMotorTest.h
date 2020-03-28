#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <stdlib.h>

#include <inc/common.h>
#include <inc/steppermotor.h>

TEST(StepperMotorTest, move)
{
    StepperMotor stepperMotor;
    EXPECT_EQ(stepperMotor.getSpeed(), 0);
    ASSERT_FALSE(stepperMotor.isThreadActive());

    U16 expectedSpeed = StepperMotor::MIN_SPEED;
    StepperMotorDirection expectedDirection = FORWARD;
    stepperMotor.move(expectedDirection, expectedSpeed);
    ASSERT_TRUE(stepperMotor.isThreadActive());
    EXPECT_EQ(stepperMotor.getSpeed(), expectedSpeed);
    EXPECT_EQ(stepperMotor.getDirection(), expectedDirection);
}
