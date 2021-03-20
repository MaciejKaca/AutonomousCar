#pragma once

#include "Base/StepperMotorShellBase.h"
#include "steppermotor.h"
#include "lights.h"
#include "SafetySystem.h"

class StepperMotorShell : StepperMotorShellBase
{
public:
    StepperMotorShell(Lights* _lights);
    ~StepperMotorShell();
    void move(const StepperMotorDirection direction, const U16 speed, const ControlPiority _controlPriority) override;
    void brake() override;
    void switchOff() override;
    void makeStep(const StepperMotorDirection direction, const U16 speed) override;
    void setSafetySystem(SafetySystem *_safetySystem);
    void setAllowedDirection(AllowedDirection _allowedDirection);
    void setControlPriority(ControlPiority _controlPiority);

    const double& getSpeed() override;
    const StepperMotorDirection &getDirection() override;
    AllowedDirection getAllowedDirection();

    static const U8 MAX_SPEED = StepperMotor::MAX_SPEED;
    static const U8 MIN_SPEED = StepperMotor::MIN_SPEED;

private:
    Lights* lights;
    StepperMotor* stepperMotor;
    SafetySystem* safetySystem;
    AllowedDirection allowedDirection;
    ControlPiority controlPriority;

    std::mutex controlPiority_mutex;
    std::mutex allowedDirection_mutex;

    bool validateSpeed(const U8& speed);
};
