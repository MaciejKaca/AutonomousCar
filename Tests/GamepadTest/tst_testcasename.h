#ifndef TST_TESTCASENAME_H
#define TST_TESTCASENAME_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(Gamepad, TestCaseName)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_TESTCASENAME_H
