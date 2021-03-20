#pragma once

#include "common.h"
#include "serialport.h"
#include "Base/ServoBase.h"

#include <future>

class StepperMotorShell;

class Servo : ServoBase
{
public:
    Servo(SerialPort* _serialPort, StepperMotorShell* _stepperMotor);
    ~Servo();
    void turn(const S8 &_angle, ControlPiority _controlPriority) override;
    void setNewCenter(const S8 &_angle) override;
    void setPriority(ControlPiority _controlPriority);
    const S8 &getAngle() override;

    static const S8 MIN_ANGLE = -45;
    static const S8 MAX_ANGLE = 45;

private:
    SerialPort* serialPort;
    StepperMotorShell* stepperMotor;
    S8 angle;
    std::future<void> centerWheelsThread;
    volatile bool cancelCenter;
    ControlPiority controlPriority;
    std::mutex controlPriority_mutex;

    const U8 WAIT_BEFORE_OFFSET = 200; //milliseconds
    const S8 FROM_LEFT_OFFSET_STAND = 10;
    const S8 FROM_LEFT_OFFSET_MOVING = 3;
    const S8 FROM_RIGHT_OFFSET_STAND = -15;
    const S8 FROM_RIGHT_OFFSET_MOVING = -3;

    void addOffset();
    bool validateAngle(const S8 _angle);
    void turn(const S8 &_angle);
};
