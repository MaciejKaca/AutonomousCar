#include <inc/servo.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include <inc/StepperMotorShell.h>

Servo::Servo(SerialPort* _serialPort, StepperMotorShell* _stepperMotor) : serialPort(_serialPort), stepperMotor(_stepperMotor)
{
    qInfo("in Servo, initializing constructor");
    angle = 0;
    cancelCenter = false;
    controlPriority = PRIORITY_MANUAL;
    centerWheelsThread = std::async(std::launch::async, &Servo::addOffset, this);
}

Servo::~Servo()
{
    qInfo("in Servo::~Servo, destructor called");
    this->turn(0);
}

ServoBase::~ServoBase(){}

const S8 &Servo::getAngle()
{
    return angle;
}

void Servo::addOffset()
{
    U8 center = 0;
    LightsAndServoMsg message;
    message.device = SERVO;
    message.servoInfo.command = TURN;
    if(angle == center)
    {
        message.servoInfo.degrees = center;
        serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));
        angle = center;
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_BEFORE_OFFSET));
        if(!cancelCenter)
        {
            if(angle > center) //Turn from right
            {
                if(stepperMotor->getSpeed() > 0)
                {
                    message.servoInfo.degrees = FROM_RIGHT_OFFSET_MOVING;
                }
                else
                {
                    message.servoInfo.degrees = FROM_RIGHT_OFFSET_STAND;
                }
                serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));
            }
            else if(angle < center) //Turn from left
            {
                if(stepperMotor->getSpeed() > 0)
                {
                    message.servoInfo.degrees = FROM_LEFT_OFFSET_MOVING;
                }
                else
                {
                    message.servoInfo.degrees = FROM_LEFT_OFFSET_STAND;
                }
                serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_BEFORE_OFFSET));
            if(!cancelCenter)
            {
                message.servoInfo.degrees = center;
                serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));
                angle = center;
            }
        }
    }
}

void Servo::turn(const S8 &_angle)
{
    if(validateAngle(_angle))
    {
        LightsAndServoMsg message;
        message.device = SERVO;
        message.servoInfo.command = TURN;
        message.servoInfo.degrees = _angle;
        serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));

        if(_angle != 0)
        {
            angle = _angle;
            cancelCenter = true;
        }
        else
        {
            cancelCenter = false;
            auto status = centerWheelsThread.wait_for(std::chrono::milliseconds(0));
            if (status == std::future_status::ready)
            {
                centerWheelsThread.get();
                centerWheelsThread = std::async(std::launch::async, &Servo::addOffset, this);
            }
        }
    }
    else
    {
        qWarning("in Servo::turn(): angle not valid");
    }
}

void Servo::turn(const S8 &_angle, ControlPiority _controlPriority)
{
    std::lock_guard<std::mutex> lock(controlPriority_mutex);
    if(_controlPriority >= controlPriority)
    {
        turn(_angle);
    }
}

void Servo::setNewCenter(const S8 &_angle)
{
    if(validateAngle(_angle))
    {
        LightsAndServoMsg message;
        message.device = SERVO;
        message.servoInfo.command = SET_NEW_ZERO;
        message.servoInfo.degrees = _angle;

        qInfo() << "in Servo::setNewCenter(): setting new center = " << angle << "degrees";

        serialPort->send((U8*)&message, sizeof(LightsAndServoMsg));
    }
    else
    {
        qWarning("in Servo::setNewCenter(): angle not valid");
    }
}

bool Servo::validateAngle(const S8 _angle)
{
    if(MIN_ANGLE<=_angle && _angle<=MAX_ANGLE)
    {
        return true;
    }
    else
    {
        qWarning() << "in Servo::validateAngle(): Angle out of range = " << _angle;
        return false;
    }
}

void Servo::setPriority(ControlPiority _controlPriority)
{
    std::lock_guard<std::mutex> lock(controlPriority_mutex);
    controlPriority = _controlPriority;
    qInfo() << "in Servo::~setPriority, priority set to: " << controlPriority;
}
