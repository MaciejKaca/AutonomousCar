#include <inc/common.h>
#include <inc/SafetySystem.h>
#include <inc/StepperMotorShell.h>
#include <inc/servo.h>

#include <cmath>

struct ObstacleDetectionInfo
{
    U16 distance;
    U8 speed;
    U16 safeDistance;
    S8 servoAngle;
    SensorAlignment sensorAlignment;
    bool isDriveAllowed;
};

SafetySystem::SafetySystem(Servo* _servo, StepperMotorShell* _stepperMotor,
                           DistanceSensors* _distanceSensors)
{
    qInfo("in SafetySystem, initializing constructor");
    servo = _servo;
    stepperMotor = _stepperMotor;
    distanceSensors = _distanceSensors;
    isSafetySystemForwardDriveThreadActive = false;
    isSafetySystemBackwardDriveThreadActive = false;
    safetySystemForwardDriveThread = std::async(std::launch::async, &SafetySystem::checkSafety,
                                                this, SENSOR_SET_FORWARD_DRIVE, &isSafetySystemForwardDriveThreadActive);
    safetySystemBackwardDriveThread = std::async(std::launch::async, &SafetySystem::checkSafety,
                                                 this, SENSOR_SET_BACKWARD_DRIVE, &isSafetySystemBackwardDriveThreadActive);
}

SafetySystem::~SafetySystem()
{
    qInfo("in SafetySystem::~SafetySystem, destructor called");
    stopThread();
}

void SafetySystem::startThread()
{
    if(stepperMotor->getDirection() == DIRECTION_FORWARD)
    {
        if (!isSafetySystemForwardDriveThreadActive)
        {
            safetySystemForwardDriveThread.get();
            isSafetySystemForwardDriveThreadActive = true;
            safetySystemForwardDriveThread = std::async(std::launch::async, &SafetySystem::checkSafety,
                                                        this, SENSOR_SET_FORWARD_DRIVE, &isSafetySystemForwardDriveThreadActive);
            qInfo("in SafetySystem::startThread, thread launched for forward drive");
        }
    }
    else
    {
        if (!isSafetySystemBackwardDriveThreadActive)
        {
            safetySystemBackwardDriveThread.get();
            isSafetySystemBackwardDriveThreadActive = true;
            safetySystemBackwardDriveThread = std::async(std::launch::async, &SafetySystem::checkSafety,
                                                         this, SENSOR_SET_BACKWARD_DRIVE, &isSafetySystemBackwardDriveThreadActive);
            qInfo("in SafetySystem::startThread, thread launched for backward drive");
        }
    }
}

void SafetySystem::stopThread()
{
    qInfo("in SafetySystem::stopThread, stopping thread");

    if(isSafetySystemForwardDriveThreadActive)
    {
        qInfo("in SafetySystem::stopThread, waiting for ForwardDriveThread");
        isSafetySystemForwardDriveThreadActive = false;
        safetySystemForwardDriveThread.get();
        qInfo("in SafetySystem::stopThread, ForwardDriveThread finished");
    }

    if(isSafetySystemBackwardDriveThreadActive)
    {
        qInfo("in SafetySystem::stopThread, waiting for ackwardDriveThrea");
        isSafetySystemBackwardDriveThreadActive = false;
        safetySystemBackwardDriveThread.get();
        qInfo("in SafetySystem::stopThread, ackwardDriveThrea finished");
    }
}

void SafetySystem::checkSafety(std::vector<SensorAlignment> sensorSet, bool* isThreadActive)
{
    ObstacleDetectionInfo obstacleInfo;
    StepperMotorDirection checkSafetyDirection;
    if(sensorSet == SENSOR_SET_FORWARD_DRIVE)
    {
        checkSafetyDirection = DIRECTION_FORWARD;
    }
    else
    {
        checkSafetyDirection = DIRECTION_BACKWARD;
    }

    if(*isThreadActive)
    {
        distanceSensors->addSensorToQueue(sensorSet);

        U8 speed = static_cast<U8>(stepperMotor->getSpeed());
        while(*isThreadActive && speed > 0)
        {
            for(auto sensorAlignment : sensorSet)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));

                if(speed > 0)
                {
                    obstacleInfo = checkSensor(sensorAlignment);
                    if(obstacleInfo.isDriveAllowed == false)
                    {
                        AllowedDirection allowedDirection = stepperMotor->getAllowedDirection();
                        if(checkSafetyDirection == DIRECTION_FORWARD)
                        {
                            allowedDirection.forward = false;
                        }
                        else
                        {
                            allowedDirection.backward = false;
                        }
                        stepperMotor->setAllowedDirection(allowedDirection);
                        stepperMotor->brake();
                    }
                    speed = stepperMotor->getSpeed();
                }
                else
                {
                    break;
                }
            }
        }

        if(obstacleInfo.isDriveAllowed == false)
        {
            qWarning() << "in SafetySystem::checkSafety, obstacle detected. Sensor: " << obstacleInfo.sensorAlignment <<
                       "Distance: " << obstacleInfo.distance << " Allowed distance: " << obstacleInfo.safeDistance <<
                       " Speed: " << obstacleInfo.speed << " Servo Angle: " << obstacleInfo.servoAngle;

            while(*isThreadActive)
            {
                U8 numberOfSensorsAllowingToDrive = 0;
                for(auto sensorAlignment : sensorSet)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
                    obstacleInfo = checkSensor(sensorAlignment);

                    if(obstacleInfo.isDriveAllowed == true)
                    {
                        numberOfSensorsAllowingToDrive++;
                    }
                }

                if(numberOfSensorsAllowingToDrive == sensorSet.size())
                {
                    servo->setPriority(PRIORITY_MANUAL);
                    AllowedDirection allowedDirection = stepperMotor->getAllowedDirection();
                    if(checkSafetyDirection == DIRECTION_FORWARD)
                    {
                        allowedDirection.forward = true;
                    }
                    else
                    {
                        allowedDirection.backward = true;
                    }
                    stepperMotor->setAllowedDirection(allowedDirection);

                    if(stepperMotor->getSpeed() == 0)
                    {
                        stepperMotor->switchOff();
                    }
                    break;
                }
            }
        }
        else
        {
            qInfo() << "in SafetySystem::checkSafety, endded due speed = 0, direction = " << checkSafetyDirection;
        }

        qInfo() << "in SafetySystem::checkSafety, thread endded, direction = " << checkSafetyDirection;
        *isThreadActive = false;
        distanceSensors->removeSensorFromQueue(sensorSet);
    }
}

ObstacleDetectionInfo SafetySystem::checkSensor(SensorAlignment sensorAlignment)
{
    ObstacleDetectionInfo obstacleInfo;
    obstacleInfo.speed = static_cast<U8>(ceil(stepperMotor->getSpeed()));
    obstacleInfo.distance = distanceSensors->getDistance(sensorAlignment);
    obstacleInfo.servoAngle = servo->getAngle();
    obstacleInfo.safeDistance = 5;
    obstacleInfo.sensorAlignment = sensorAlignment;
    obstacleInfo.isDriveAllowed = true;

    switch (sensorAlignment)
    {
    default:
        if(obstacleInfo.distance <= obstacleInfo.safeDistance)
        {
            obstacleInfo.isDriveAllowed = false;
        }
        break;
    }

    return obstacleInfo;
}
