#pragma once

#include "common.h"
#include "serialport.h"

class Servo
{
public:
    Servo(SerialPort * _serialPort);
    void turn(const S8 _angle);
    void setNewCenter(const S8 _angle);
    S8 getAngle();
    const S8 MIN_ANGLE = -40;
    const S8 MAX_ANGLE = 40;

private:
    SerialPort * serialPort;
    S8 angle;
    bool validateAngle(const S8 _angle);
};
