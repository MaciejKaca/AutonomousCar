#pragma once

#include <inc/common.h>
#include <inc/Base/StepperMotorBase.h>

class StepperMotorStub : StepperMotorBase
{
public:
    StepperMotorStub();
    void move(const StepperMotorDirection _direction, const U16 _speed) override;
    void brake() override;
    void switchOff() override;
    void makeStep(const StepperMotorDirection _direction, const U16 _speed) override;
    const double &getSpeed() override;
    const StepperMotorDirection &getDirection() override;

private:
    double speed;
    StepperMotorDirection direction;
};

StepperMotorStub::StepperMotorStub()
{
    speed = 0;
    direction = DIRECTION_FORWARD;
}

void StepperMotorStub::move(const StepperMotorDirection _direction, const U16 _speed)
{
    direction = _direction;
    speed = _speed;
}

void StepperMotorStub::brake()
{
    speed = 0;
}

void StepperMotorStub::switchOff()
{
    speed = 0;
}

void StepperMotorStub::makeStep(const StepperMotorDirection _direction, const U16 _speed)
{
}

const double &StepperMotorStub::getSpeed()
{
    return speed;
}

const StepperMotorDirection &StepperMotorStub::getDirection()
{
    return direction;
}
