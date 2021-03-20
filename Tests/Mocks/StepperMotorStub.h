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
    bool makeStep(const StepperMotorDirection _direction, const U16 _speed) override;
    const bool &isThreadActive() override;
    const U16 &getSpeed() override;
    const StepperMotorDirection &getDirection() override;

private:
    U16 speed;
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

bool StepperMotorStub::makeStep(const StepperMotorDirection _direction, const U16 _speed)
{
}

const bool &StepperMotorStub::isThreadActive()
{
}

const U16 &StepperMotorStub::getSpeed()
{
    return speed;
}

const StepperMotorDirection &StepperMotorStub::getDirection()
{
    return direction;
}
