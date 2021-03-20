#pragma once

#include <inc/common.h>

class DistanceSensorsBase
{
public:
    virtual U16 getDistance(SensorAlignment sensorAlignment) = 0;
    virtual void addSensorToQueue(std::vector<SensorAlignment> sensorsAlignment) = 0;
    virtual void removeSensorFromQueue(std::vector<SensorAlignment> sensorsAlignment) = 0;
    virtual ~DistanceSensorsBase() = 0;
};
