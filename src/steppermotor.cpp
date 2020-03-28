#include <inc/steppermotor.h>
#include <inc/common.h>

#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include <QDebug>

StepperMotor::StepperMotor()
{
    qInfo("in StepperMotor, initializing constructor");
    if(wiringPiSetup() < 0)
    {
        qCritical("in StepperMotor::StepperMotor(): Failed to launch wiringPi");
        exit(EXIT_BY_FAILED_GPIO);
    }
    else
    {
        qInfo("in StepperMotor::StepperMotor(): Lanuch wiringPi succes");
    }

    pinMode(STEPPER_DIRECTION_PIN, OUTPUT);
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_STEP_PIN, OUTPUT);

    digitalWrite(STEPPER_DIRECTION_PIN, LOW);
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
    digitalWrite(STEPPER_STEP_PIN, LOW);

    threadStatus = false;
    speed = 0;
}

StepperMotor::~StepperMotor()
{
    qInfo() << "in StepperMotor destructor called";
    switchOff();
    checkAndStopThread();
}

bool StepperMotor::validateSpeed(const U16 &_speed)
{
    if(_speed > MAX_SPEED || _speed < MIN_SPEED)
    {
        qWarning() << "in StepperMotor::validateSpeed(): speed out of range = " << _speed;
        return false;
    }
    else
    {
        return true;
    }
}

bool StepperMotor::makeStep(const StepperMotorDirection _direction, const U16 _speed)
{
    U16 delay = MAX_DELAY - _speed*((MAX_DELAY-MIN_DELAY)/(MAX_SPEED-MIN_SPEED));

    if(_direction == FORWARD)
    {
        digitalWrite(STEPPER_DIRECTION_PIN, HIGH);
    }
    else if(_direction == BACKWARD)
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
    qInfo("in StepperMotor::brake(): braking");
    speed = 0;
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
}

void StepperMotor::switchOff()
{
    checkAndStopThread();
    qInfo("in StepperMotor::swithOff(): switchOff off stepper");
    speed = 0;
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
}

void StepperMotor::constantMovement()
{
    while(threadStatus == true)
    {
        makeStep(direction, speed);
    }
}

void StepperMotor::checkAndStopThread()
{
    if(threadStatus == true)
    {
        qInfo("in StepperMotor::checkAndStopThread(): stopping thread");
        threadStatus = false;
        stepperThread.join();
    }
}

void StepperMotor::move(const StepperMotorDirection _direction,  const U16 _speed)
{
    if(validateSpeed(_speed))
    {
        speed = _speed;
        direction = _direction;

        if(threadStatus == false)
        {
            threadStatus = true;
            qInfo("in StepperMotor::move(): launching thread for constantMovement");
            stepperThread = std::thread(&StepperMotor::constantMovement, this);
        }
    }
}

const bool &StepperMotor::isThreadActive()
{
    return threadStatus;
}

const U16 &StepperMotor::getSpeed()
{
    return speed;
}

const StepperMotorDirection &StepperMotor::getDirection()
{
    return direction;
}
