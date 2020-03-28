#include <Suites/LightsTest.h>
#include <Suites/ServoTest.h>
#include <Suites/StepperMotorTest.h>
#include <Suites/GamepadTest.h>

#include <gtest/gtest.h>
#include <inc/logutils.h>

#define TEST

int main(int argc, char *argv[])
{
    LOGUTILS::initLogging();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
