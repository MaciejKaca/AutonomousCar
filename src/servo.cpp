#include <inc/servo.h>

Servo::Servo(SerialPort * _serialPort) : serialPort(_serialPort)
{
    qInfo("in Servo, initializing constructor");
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

        qInfo() << "in Servo::turn(): turning servo " << angle << "degrees";

        serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
    }
    else
    {
        qWarning("in Servo::turn(): angle not valid");
    }
}

void Servo::setNewCenter(const S8 _angle)
{
    if(validateAngle(_angle))
    {
        LightsAndServoMsg message;
        message.device = SERVO;
        message.servoInfo.command = SET_NEW_ZERO;
        message.servoInfo.degrees = _angle;

        qInfo() << "in Servo::setNewCenter(): setting new center = " << angle << "degrees";

        serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
    }
    else
    {
        qWarning("in Servo::setNewCenter(): angle not valid");
    }
}

bool Servo::validateAngle(const S8 _angle)
{
    if(MIN_ANGLE <= _angle && _angle <= MAX_ANGLE)
    {
        qInfo() << "in Servo::validateAngle(): Angle valid = " << _angle;
        return true;
    }
    else
    {
        qWarning() << "in Servo::validateAngle(): Angle out of range = " << _angle;
        return false;
    }
}
