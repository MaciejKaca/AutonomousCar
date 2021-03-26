#pragma once

#include <inc/common.h>

class SerialPortBase
{
public:
    virtual void send(U8 *buffer,const S16 &size) = 0;
    virtual int read(U8 *buffer,const S16 &size) = 0;
    virtual ~SerialPortBase() = 0;
};
