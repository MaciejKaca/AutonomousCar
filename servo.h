#ifndef SERVO_H
#define SERVO_H

#include "common.h"
#include "serialport.h"

class Servo
{
public:
    Servo(SerialPort * _serialPort);
    void turn(S8 degrees);
    void setNewCenter(S8 _angle);
    S8 getAngle();
    const S8 MIN_ANGLE = -40;
    const S8 MAX_ANGLE = 40;

private:
    SerialPort * serialPort;
    S8 angle;
    bool validateAngle(S8 _angle);
};

#endif // SERVO_H
