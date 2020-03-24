#pragma once

#include <inc/common.h>

class SerialPortBase
{
public:
    virtual bool send(U8 *buffer,const S16 &size) = 0;
    virtual void read(U8 *buffer,const S16 &size) = 0;
};
