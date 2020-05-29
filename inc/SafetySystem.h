#pragma once

#include <future>
#include <array>
#include <inc/common.h>
#include "DistanceSensors.h"

class Servo;
class DistanceSensors;
class StepperMotorShell;
struct ObstacleDetectionInfo;

class SafetySystem
{
public:
    SafetySystem(Servo* _servo, StepperMotorShell* _stepperMotor,
                 DistanceSensors* _distanceSensors);
    void startThread();
    void stopThread();
    ~SafetySystem();

private:

    DistanceSensors *distanceSensors;
    StepperMotorShell *stepperMotor;
    Servo *servo;

    std::future<void> safetySystemForwardDriveThread;
    std::future<void> safetySystemBackwardDriveThread;
    bool isSafetySystemForwardDriveThreadActive;
    bool isSafetySystemBackwardDriveThreadActive;

    const std::vector<SensorAlignment> SENSOR_SET_FORWARD_DRIVE{ {SENSOR_FRONT, SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT} };
    const std::vector<SensorAlignment> SENSOR_SET_BACKWARD_DRIVE{ {SENSOR_REAR, SENSOR_REAR_LEFT, SENSOR_REAR_RIGHT} };
    const U8 REFRESH_TIME = 10; //milliseonds

    void checkSafety(std::vector<SensorAlignment> sensorSet, bool* threadFlag);
    void clearSensors();
    ObstacleDetectionInfo checkSensor(SensorAlignment sensorAlignment);
};
