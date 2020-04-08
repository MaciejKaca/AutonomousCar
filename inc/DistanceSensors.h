#pragma once

#include "Base/DistanceSensorsBase.h"
#include <future>
#include <array>

class DistanceSensors : DistanceSensorsBase
{
public:
    DistanceSensors();
    const U16 &getDistance(const SensorAlignment &sensorAlignment) override;
    void stopThread() override;
    void startThread() override;
    ~DistanceSensors();
private:
    static const U8 NUMBER_OF_SENSORS = 8;
    std::array<U16, NUMBER_OF_SENSORS> measurements;
    std::future<void> measurementThread;
    void collectMeasurements();
    void makeMeasurement(const U8 &sensorAlignment);
    bool terminateThread;
    const U8 DISTANCE_SENSORS_PINS[NUMBER_OF_SENSORS] = {25, 24, 23, 21, 3, 2};
    const U8 TRIGGER_SENSOR_PIN = 26;
    const U8 DELAY_BETWEEN_MEASUREMENTS = 50; //milliseconds
    const U8 TRIGGER_DELAY = 10; //microseconds
    const U16 SENSOR_TIMEOUT = 11630; //microseconds
    const U8 NUMBER_OF_MEASUREMENTS_PER_SENSOR = 5;
    U32 pulseIn(const U8 &sensorAlignment);
    const float SOUND_SPEED = 0.034;
    U16 mostFrequent(U16 array[]);
    void trigger();
};
