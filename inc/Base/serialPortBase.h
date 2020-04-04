#pragma once

#include <inc/common.h>

class SerialPortBase
{
public:
    virtual void send(U8 *buffer,const S16 &size) = 0;
    virtual void read(U8 *buffer,const S16 &size) = 0;
    virtual const bool &isSerialOpen() = 0;
    virtual ~SerialPortBase() = 0;
};
