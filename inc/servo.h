#pragma once

#include "common.h"
#include "serialport.h"
#include "Base/ServoBase.h"

class Servo : ServoBase
{
public:
    Servo(SerialPort * _serialPort);
    void turn(const S8 &_angle) override;
    void setNewCenter(const S8 &_angle) override;
    const S8 &getAngle() override;
    static const S8 MIN_ANGLE = -40;
    static const S8 MAX_ANGLE = 40;

private:
    SerialPort * serialPort;
    S8 angle;
    bool validateAngle(const S8 _angle);
};
