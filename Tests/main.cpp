#include <Suites/LightsTest.h>

#include <gtest/gtest.h>
#include <inc/logutils.h>

int main(int argc, char *argv[])
{
    LOGUTILS::initLogging();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
