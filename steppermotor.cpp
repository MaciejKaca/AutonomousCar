#include "steppermotor.h"
#include "common.h"

#include <pigpio.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <future>
#include <QDebug>

StepperMotor::StepperMotor()
{
    if(gpioInitialise() < 0)
    {
        qCritical( "GPIO Failed");
        exit(EXIT_BY_FAILED_GPIO);
    }

    gpioSetMode(STEPPER_DIRECTION_PIN, PI_OUTPUT);
    gpioSetMode(STEPPER_ENABLE_PIN, PI_OUTPUT);
    gpioSetMode(STEPPER_STEP_PIN, PI_OUTPUT);

    gpioWrite(STEPPER_DIRECTION_PIN, false);
    gpioWrite(STEPPER_ENABLE_PIN, false);
    gpioWrite(STEPPER_STEP_PIN, false);

    threadActive = false;
}

void StepperMotor::validateSpeed(uint16_t &speed)
{
    if(speed > MAX_SPEED)
        speed = MAX_SPEED;

    if(speed < MIN_SPEED)
        speed = MIN_SPEED;

    delay = MIN_SPEED + (MAX_SPEED - speed);
}

bool StepperMotor::makeStep(StepperMotorDirection direction, uint16_t speed)
{
    validateSpeed(speed);

    if(direction == FORWARD)
    {
        gpioWrite(STEPPER_DIRECTION_PIN, false);
    }
    else if(direction == BACKWARD)
    {
        gpioWrite(STEPPER_DIRECTION_PIN, true);
    }

    gpioWrite(STEPPER_ENABLE_PIN, true);

    gpioWrite(STEPPER_STEP_PIN, false);
    std::this_thread::sleep_for(std::chrono::microseconds(delay));
    gpioWrite(STEPPER_STEP_PIN, true);
    std::this_thread::sleep_for(std::chrono::microseconds(delay));

    return true;
}

void StepperMotor::brake()
{
    threadActive = false;
    gpioWrite(STEPPER_ENABLE_PIN, true);
}

void StepperMotor::swithOff()
{
    threadActive = false;
    gpioWrite(STEPPER_ENABLE_PIN, false);
}

void StepperMotor::constantMovement()
{
    threadActive = true;

    while(threadActive == true)
    {
        makeStep(direction, speed);
    }
}

void StepperMotor::move(StepperMotorDirection _direction, uint16_t _speed)
{
    direction = _direction;
    speed = _speed;

    if(threadActive == false)
    {
        auto gamepadThread = std::async(std::launch::async, &StepperMotor::constantMovement, this);
    }
}
