#include <inc/StepperMotorShell.h>
#include <inc/SafetySystem.h>

#include <mutex>

StepperMotorShell::StepperMotorShell(Lights* _lights)
{
    stepperMotor = new(StepperMotor);
    lights = _lights;
    allowedDirection.forward = true;
    allowedDirection.backward = true;
    controlPriority = PRIORITY_MANUAL;
}

StepperMotorShell::~StepperMotorShell()
{
    delete stepperMotor;
}

StepperMotorShellBase::~StepperMotorShellBase()
{}

void StepperMotorShell::move(const StepperMotorDirection direction, const U16 speed,  const ControlPiority _controlPriority)
{
    std::lock_guard<std::mutex> lock(controlPiority_mutex);
    if(_controlPriority >= controlPriority)
    {
        std::lock_guard<std::mutex> allowedDirection_lock(allowedDirection_mutex);
        if((direction == DIRECTION_FORWARD && allowedDirection.forward) ||
                (direction == DIRECTION_BACKWARD && allowedDirection.backward))
        {
            if(validateSpeed(speed))
            {
                stepperMotor->move(direction, speed);
                safetySystem->startThread();

                if(lights->getBrakeLightsStatus() == BRAKE_LIGHT_STOP)
                {
                    lights->setBrakeLights(BRAKE_LIGHT_OFF);
                }

                if(direction == DIRECTION_FORWARD &&
                        lights->getReverseLightStatus() == REVERSE_LIGHT_ON)
                {
                    lights->setReverseLight(REVERSE_LIGHT_OFF);
                }

                if(direction == DIRECTION_BACKWARD &&
                        lights->getReverseLightStatus() == REVERSE_LIGHT_OFF)
                {
                    lights->setReverseLight(REVERSE_LIGHT_ON);
                }
            }
        }
    }
}

void StepperMotorShell::brake()
{
    stepperMotor->brake();
    lights->setBrakeLights(BRAKE_LIGHT_STOP);
}

void StepperMotorShell::switchOff()
{
    stepperMotor->switchOff();
    if(lights->getBrakeLightsStatus() == BRAKE_LIGHT_STOP)
    {
        lights->setBrakeLights(BRAKE_LIGHT_OFF);
    }
}

void StepperMotorShell::makeStep(const StepperMotorDirection direction, const U16 speed)
{
    stepperMotor->makeStep(direction, speed);
}

const double& StepperMotorShell::getSpeed()
{
    return stepperMotor->getSpeed();
}

const StepperMotorDirection& StepperMotorShell::getDirection()
{
    return stepperMotor->getDirection();
}

void StepperMotorShell::setAllowedDirection(AllowedDirection _allowedDirection)
{
    std::lock_guard<std::mutex> allowedDirection_lock(allowedDirection_mutex);
    allowedDirection = _allowedDirection;
}

bool StepperMotorShell::validateSpeed(const U8& speed)
{
    if(speed > MAX_SPEED || speed <= MIN_SPEED)
    {
        qWarning() << "in StepperMotorShell::validateSpeed(): speed out of range = " << speed;
        return false;
    }
    else
    {
        return true;
    }
}

AllowedDirection StepperMotorShell::getAllowedDirection()
{
    std::lock_guard<std::mutex> allowedDirection_lock(allowedDirection_mutex);
    return allowedDirection;
}

void StepperMotorShell::setSafetySystem(SafetySystem *_safetySystem)
{
    safetySystem = _safetySystem;
}

void StepperMotorShell::setControlPriority(ControlPiority _controlPiority)
{
    std::lock_guard<std::mutex> lock(controlPiority_mutex);
    controlPriority = _controlPiority;
}
