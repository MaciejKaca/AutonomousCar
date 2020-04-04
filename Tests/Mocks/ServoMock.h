#pragma once

#include <gmock/gmock.h>
#include <inc/Base/ServoBase.h>

class ServoMock : public ServoBase
{
public:
    MOCK_METHOD(void, turn, (const S8 &_angle), (override));
    MOCK_METHOD(void, setNewCenter, (const S8 &_angle), (override));
    MOCK_METHOD(const S8&, getAngle, (), (override));
};
