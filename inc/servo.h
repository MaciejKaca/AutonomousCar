#pragma once

#include "common.h"
#include "serialport.h"
#include "steppermotor.h"
#include "Base/ServoBase.h"

#include <future>

class Servo : ServoBase
{
public:
    Servo(SerialPort* _serialPort, StepperMotor* _stepperMotor);
    ~Servo();
    void turn(const S8 &_angle) override;
    void setNewCenter(const S8 &_angle) override;
    const S8 &getAngle() override;
    static const S8 MIN_ANGLE = -40;
    static const S8 MAX_ANGLE = 40;

private:
    SerialPort* serialPort;
    StepperMotor* stepperMotor;
    S8 angle;
    bool validateAngle(const S8 _angle);
    std::future<void> centerWheelsThread;
    void addOffset();
    volatile bool cancelCenter;
    const U8 WAIT_BEFORE_OFFSET = 200; //milliseconds
    const S8 FROM_LEFT_OFFSET_STAND = 9;
    const S8 FROM_LEFT_OFFSET_MOVING = 6;
    const S8 FROM_RIGHT_OFFSET_STAND = -6;
    const S8 FROM_RIGHT_OFFSET_MOVING = -3;
};
