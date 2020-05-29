#pragma once

#include "Base/DistanceSensorsBase.h"
#include <future>
#include <array>
#include <cstdint>

class DistanceSensors : DistanceSensorsBase
{
public:
    DistanceSensors();
    U16 getDistance(SensorAlignment sensorAlignment) override;
    void addSensorToQueue(std::vector<SensorAlignment> sensorsAlignment) override;
    void removeSensorFromQueue(std::vector<SensorAlignment> sensorsAlignment) override;

    static const U8 NUMBER_OF_SENSORS = 8;
    static const U8 MIN_DISTANCE = 0;
    static const U16 MAX_DISTANCE = 400;
    ~DistanceSensors();
private:
    struct EchoTriggerPinsS
    {
        U8 echoAddress;
        U8 triggerPin;
    };
    struct SensorState
    {
        U16 distance;
        bool sensorActive;
    };

    const U32 DELAY_BETWEEN_MEASUREMENTS = 30; //milliseconds
    const U8 TRIGGER_DURATION = 10; //microseconds
    const U16 SENSOR_TIMEOUT = 20411; //microseconds
    const U16 INVALID_PULSE_IN = INT16_MAX; //microseconds
    const U8 NUMBER_OF_MEASUREMENTS_PER_SENSOR = 1;
    const float SOUND_SPEED = 0.034;
    const U8 COMMON_ECHO = 7;
    const U8 DISTANCE_THRESHOLD = 15;
    const std::array<U8,3> MULTIPLEXER_ADDRESS_PINS{ {1, 4, 5} };
    const U8 FRONT_SENSOR_OFFSET = 4; //cm
    const std::map<SensorAlignment, EchoTriggerPinsS> DISTANCE_SENSORS_PINS = {{SENSOR_FRONT, {.echoAddress = 0,.triggerPin = 23}},
                                                                               {SENSOR_FRONT_LEFT, {.echoAddress = 5, .triggerPin = 21}},
                                                                               {SENSOR_FRONT_RIGHT, {.echoAddress = 6, .triggerPin = 22}},
                                                                               {SENSOR_REAR, {.echoAddress = 3, .triggerPin = 25}},
                                                                               {SENSOR_REAR_LEFT, {.echoAddress = 4, .triggerPin = 24}},
                                                                               {SENSOR_REAR_RIGHT, {.echoAddress = 1, .triggerPin = 3}},
                                                                               {SENSOR_LEFT_SIDE, {.echoAddress = 2, .triggerPin = 2}},
                                                                               {SENSOR_RIGHT_SIDE, {.echoAddress = 7, .triggerPin = 0}}};
    SensorState measurements[NUMBER_OF_SENSORS];
    std::future<void> measurementThread;
    bool isDistanceSensorThreadActive;
    std::map<SensorAlignment, U16> sensorQueueCounter;
    std::mutex measurements_mutex;
    std::mutex sensorQueue_mutex;

    void collectMeasurements();
    void makeMeasurement(const SensorAlignment &sensorAlignment);
    inline U32 pulseIn(const SensorAlignment &sensorAlignment);
    U16 mostFrequent(const U16 array[]);
    void trigger(const SensorAlignment &sensorAlignment);
    void setMultiplexerAddress(const U8 &address);
    void stopThread();
    void startThread();
    void startAndWaitUntilSensorIsActive(std::vector<SensorAlignment> sensorsAlignment);
};
