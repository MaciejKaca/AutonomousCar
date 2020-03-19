#include <inc/servo.h>

Servo::Servo(SerialPort * _serialPort) : serialPort(_serialPort)
{
    angle = 0;
}


S8 Servo::getAngle()
{
    return angle;
}

void Servo::turn(const S8 _angle)
{
    if(validateAngle(_angle))
    {
        angle = _angle;

        LightsAndServoMsg message;
        message.device = SERVO;
        message.servoInfo.command = TURN;
        message.servoInfo.degrees = angle;
        serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
    }
}

void Servo::setNewCenter(const S8 _angle)
{
    LightsAndServoMsg message;
    message.device = SERVO;
    message.servoInfo.command = SET_NEW_ZERO;
    message.servoInfo.degrees = _angle;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

bool Servo::validateAngle(const S8 _angle)
{
    if(MIN_ANGLE <= _angle && _angle <= MAX_ANGLE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
