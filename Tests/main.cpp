#include <Suites/LightsTest.h>
#include <Suites/ServoTest.h>
#include <Suites/StepperMotorTest.h>
#include <Suites/GamepadTest.h>

#include <gtest/gtest.h>
#include <inc/logutils.h>

int main(int argc, char *argv[])
{
    LOGUTILS::initLogging(true);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
