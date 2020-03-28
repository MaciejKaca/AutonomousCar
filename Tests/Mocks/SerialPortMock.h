#pragma once

#include <gmock/gmock.h>
#include <inc/Base/serialPortBase.h>

class SerialPortMock : public SerialPortBase
{
public:
    MOCK_METHOD(bool, send, (U8 *buffer,const S16 &size), (override));
    MOCK_METHOD(void, read, (U8 *buffer,const S16 &size), (override));
};
