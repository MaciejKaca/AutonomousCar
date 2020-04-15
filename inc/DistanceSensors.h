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
        U8 echoAddress;
        U8 triggerPin;
    };
    static const U8 NUMBER_OF_SENSORS = 8;
    const U8 DELAY_BETWEEN_MEASUREMENTS = 5; //milliseconds
    const U8 TRIGGER_DURATION = 10; //microseconds
    const U16 SENSOR_TIMEOUT = 23260; //microseconds
    const U8 NUMBER_OF_MEASUREMENTS_PER_SENSOR = 10;
    const float SOUND_SPEED = 0.034;
    const U8 COMMON_ECHO = 7;
    const U8 DISTANCE_THRESHOLD = 15;
    const U8 MIN_DISTANCE = 0;
    const U16 MAX_DISTANCE = 400;
    static const U8 NUMBER_OF_ADDRESS_PINS = 3;
    const U8 MULTIPLEXER_ADDRESS_PINS[NUMBER_OF_ADDRESS_PINS] = {1, 4, 5};
    const std::map<SensorAlignment, EchoTriggerPinsS> DISTANCE_SENSORS_PINS = {{SENSOR_FRONT, {.echoAddress = 7,.triggerPin = 2}},
                                                                               {SENSOR_FRONT_LEFT, {.echoAddress = 5, .triggerPin = 21}},
                                                                               {SENSOR_FRONT_RIGHT, {.echoAddress = 6, .triggerPin = 22}},
                                                                               {SENSOR_REAR, {.echoAddress = 3, .triggerPin = 0}},
                                                                               {SENSOR_REAR_LEFT, {.echoAddress = 4, .triggerPin = 3}},
                                                                               {SENSOR_REAR_RIGHT, {.echoAddress = 1, .triggerPin = 25}},
                                                                               {SENSOR_LEFT_SIDE, {.echoAddress = 2, .triggerPin = 23}},
                                                                               {SENSOR_RIGHT_SIDE, {.echoAddress = 0, .triggerPin = 24}}};
    std::array<U16, NUMBER_OF_SENSORS> measurements;
    std::future<void> measurementThread;
    bool terminateThread;

    void collectMeasurements();
    void makeMeasurement(const SensorAlignment &sensorAlignment);
    U32 pulseIn(const SensorAlignment &sensorAlignment);
    U16 mostFrequent(const U16 array[]);
    void trigger(const SensorAlignment &sensorAlignment);
    void setMultiplexerAddress(const U8 &address);
};
