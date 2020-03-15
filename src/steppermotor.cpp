#include "steppermotor.h"
#include "common.h"

#include <wiringPi.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <future>
#include <QDebug>

StepperMotor::StepperMotor()
{
    if(wiringPiSetup() < 0)
    {
        qCritical( "GPIO Failed");
        exit(EXIT_BY_FAILED_GPIO);
    }

    pinMode(STEPPER_DIRECTION_PIN, OUTPUT);
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_STEP_PIN, OUTPUT);

    digitalWrite(STEPPER_DIRECTION_PIN, LOW);
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
    digitalWrite(STEPPER_STEP_PIN, LOW);

    threadActive = false;
}

void StepperMotor::validateSpeed(U16 &speed)
{
    if(speed > MAX_SPEED)
        speed = MAX_SPEED;

    if(speed < MIN_SPEED)
        speed = MIN_SPEED;

    delay = MIN_SPEED + (MAX_SPEED - speed);
}

bool StepperMotor::makeStep(const StepperMotorDirection direction,  U16 speed)
{
    validateSpeed(speed);

    if(direction == FORWARD)
    {
        digitalWrite(STEPPER_DIRECTION_PIN, HIGH);
    }
    else if(direction == BACKWARD)
    {
        digitalWrite(STEPPER_DIRECTION_PIN, LOW);
    }

    digitalWrite(STEPPER_ENABLE_PIN, HIGH);

    digitalWrite(STEPPER_STEP_PIN, HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(delay));
    digitalWrite(STEPPER_STEP_PIN, LOW);
    std::this_thread::sleep_for(std::chrono::microseconds(delay));

    return true;
}

void StepperMotor::brake()
{
    checkAndStopThread();
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
}

void StepperMotor::swithOff()
{
    checkAndStopThread();
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
}

void StepperMotor::constantMovement()
{
    threadActive = true;

    while(threadActive == true)
    {
        makeStep(direction, speed);
    }
}

void StepperMotor::checkAndStopThread()
{
    if(threadActive == true)
    {
        threadActive = false;
        stepperThread.join();
    }
}

void StepperMotor::move(const StepperMotorDirection _direction,  const U16 _speed)
{
    direction = _direction;
    speed = _speed;

    if( speed > MIN_SPEED)
    {
        if(threadActive == false)
        {
            stepperThread = std::thread(&StepperMotor::constantMovement, this);
        }
    }
    else
    {
        swithOff();
    }
}
