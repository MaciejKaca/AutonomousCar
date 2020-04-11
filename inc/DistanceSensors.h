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
    struct EchoTriggerPinsS
    {
        U8 echoPin;
        U8 triggerPin;
    };
    static const U8 NUMBER_OF_SENSORS = 8;
    const U8 DELAY_BETWEEN_MEASUREMENTS = 5; //milliseconds
    const U8 TRIGGER_DURATION = 10; //microseconds
    const U16 SENSOR_TIMEOUT = 11630; //microseconds
    const U8 NUMBER_OF_MEASUREMENTS_PER_SENSOR = 5;
    const float SOUND_SPEED = 0.034;
    const std::map<SensorAlignment, EchoTriggerPinsS> DISTANCE_SENSORS_PINS = {{SENSOR_FRONT, {.echoPin = 0,.triggerPin = 1}},
                                                                               {SENSOR_FRONT_LEFT, {.echoPin = 3, .triggerPin = 5}},
                                                                               {SENSOR_FRONT_RIGHT, {.echoPin = 2, .triggerPin = 5}},
                                                                               {SENSOR_REAR, {.echoPin = 25, .triggerPin = 1}},
                                                                               {SENSOR_REAR_LEFT, {.echoPin = 23, .triggerPin = 5}},
                                                                               {SENSOR_REAR_RIGHT, {.echoPin = 24, .triggerPin = 5}},
                                                                               {SENSOR_LEFT_SIDE, {.echoPin = 22, .triggerPin = 4}},
                                                                               {SENSOR_RIGHT_SIDE, {.echoPin = 21, .triggerPin = 4}}};
    std::array<U16, NUMBER_OF_SENSORS> measurements;
    std::future<void> measurementThread;
    bool terminateThread;

    void collectMeasurements();
    void makeMeasurement(const SensorAlignment &sensorAlignment);
    U32 pulseIn(const SensorAlignment &sensorAlignment);
    U16 mostFrequent(const U16 array[]);
    void trigger(const SensorAlignment &sensorAlignment);
};
