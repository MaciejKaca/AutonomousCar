#pragma once

#include <inc/common.h>

class ServoBase
{
public:
    virtual void turn(const S8 &_angle) = 0;
    virtual void setNewCenter(const S8 &_angle) = 0;
    virtual const S8 &getAngle() = 0;
    virtual ~ServoBase() = 0;
};
