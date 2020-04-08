#pragma once

#include <inc/common.h>

class DistanceSensorsBase
{
public:
    virtual const U16 &getDistance(const SensorAlignment &sensorAlignment) = 0;
    virtual void startThread() = 0;
    virtual void stopThread() = 0;
    virtual ~DistanceSensorsBase() = 0;
};
