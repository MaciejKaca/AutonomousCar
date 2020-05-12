#include <inc/steppermotor.h>
#include <inc/common.h>

#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include <QDebug>
#include <cmath>

StepperMotor::StepperMotor()
{
    qInfo("in StepperMotor, initializing constructor");

    pinMode(STEPPER_DIRECTION_PIN, OUTPUT);
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_STEP_PIN, OUTPUT);

    digitalWrite(STEPPER_DIRECTION_PIN, LOW);
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
    digitalWrite(STEPPER_STEP_PIN, LOW);

    speed = 0;
    desiredSpeed = 0;
    terminateThread = true;

    stepperThread = std::async(std::launch::async, &StepperMotor::constantMovement, this);
}

StepperMotorBase::~StepperMotorBase(){}

StepperMotor::~StepperMotor()
{
    qInfo() << "in StepperMotor::~StepperMotor, destructor called";
    switchOff();
}

bool StepperMotor::validateSpeed(const U16 &_speed)
{
    if(_speed > MAX_SPEED || _speed <= MIN_SPEED)
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

    if(_direction == DIRECTION_FORWARD)
    {
        digitalWrite(STEPPER_DIRECTION_PIN, HIGH);
    }
    else if(_direction == DIRECTION_BACKWARD)
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
    qInfo("in StepperMotor::brake(): braking");

    stopThread();
    speed = 0;
    desiredSpeed = 0;
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
}

void StepperMotor::switchOff()
{
    qInfo("in StepperMotor::switchOff(): switchOff off stepper");

    stopThread();
    speed = 0;
    desiredSpeed = 0;
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
}

void StepperMotor::constantMovement()
{
    while(!terminateThread)
    {
        acceleration();
        makeStep(direction, speed);
    }
}


void StepperMotor::move(const StepperMotorDirection _direction,  const U16 _speed)
{
    if(validateSpeed(_speed))
    {
        desiredSpeed = _speed;
        direction = _direction;

        launchThread();
    }
}

const double& StepperMotor::getSpeed()
{
    return speed;
}

const StepperMotorDirection &StepperMotor::getDirection()
{
    return direction;
}

void StepperMotor::acceleration()
{
    if(speed == 0)
    {
        speed++;
    }
    if(speed < desiredSpeed)
    {
        if(speed < ACCEL_FUNC_SWITCH_THRES)
        {
            speed = (((speed/ACCELERATION_LINEAR)+ACCEL_STEP_RESOLUTION)*ACCELERATION_LINEAR);
        }
        else
        {
            speed = (log(exp(speed/ACCELERATION_LOG)+ACCEL_STEP_RESOLUTION)*ACCELERATION_LOG);
        }

        if(speed > desiredSpeed)
        {
            speed = desiredSpeed;
        }
    }
    else if(speed > desiredSpeed)
    {
        speed = floor(((speed/ACCELERATION_LINEAR)-ACCEL_STEP_RESOLUTION)*ACCELERATION_LINEAR);
    }
}

bool StepperMotor::isThreadRunning()
{
    auto status = stepperThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        return false;
    }
    else
    {
        return  true;
    }
}

void StepperMotor::launchThread()
{
    if(!isThreadRunning())
    {
        qInfo("in StepperMotor::launchThread(): launching thread for constantMovement");
        stepperThread.get();
        terminateThread = false;
        stepperThread = std::async(std::launch::async, &StepperMotor::constantMovement, this);
    }
}

void StepperMotor::stopThread()
{
    qInfo("in StepperMotor::stopThread(): stopping thread");
    terminateThread = true;
}
