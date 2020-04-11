#include <inc/common.h>
#include <inc/DistanceSensors.h>
#include <wiringPi.h>
#include <chrono>
#include <iostream>

#define GET_VARIABLE_NAME(Variable) (#Variable)

DistanceSensors::DistanceSensors()
{
    qInfo("in DistanceSensors, initializing constructor");
    measurements.fill(0);
    terminateThread = false;
    for(auto sensorPins : DISTANCE_SENSORS_PINS)
    {
        pinMode(sensorPins.second.echoPin, INPUT);
        pinMode(sensorPins.second.triggerPin, OUTPUT);
        digitalWrite(sensorPins.second.triggerPin, LOW);
    }
}

DistanceSensors::~DistanceSensors()
{
    qInfo("in DistanceSensors::~DistanceSensors, destructor called");
    stopThread();
}

DistanceSensorsBase::~DistanceSensorsBase(){}

const U16 &DistanceSensors::getDistance(const SensorAlignment &sensorAlignment)
{
    return measurements.at(sensorAlignment);
}

void DistanceSensors::startThread()
{
    qInfo("in DistanceSensors::startThread, launching thread");
    terminateThread = false;
    measurementThread = std::async(std::launch::async, &DistanceSensors::collectMeasurements, this);
}

void DistanceSensors::stopThread()
{
    qInfo("in DistanceSensors::stopThread, stopping thread");
    terminateThread = true;
    measurementThread.get();
}

void DistanceSensors::collectMeasurements()
{
    qInfo("in DistanceSensors::collectMeasurements, succesfull thread launching");
    while(!terminateThread)
    {
        for(auto sensorPins : DISTANCE_SENSORS_PINS)
        {
            SensorAlignment sensorAlignment = sensorPins.first;
            makeMeasurement(sensorAlignment);
        }
    }
    qInfo("in DistanceSensors::collectMeasurements, thread ended");
}

void DistanceSensors::makeMeasurement(const SensorAlignment &sensorAlignment)
{
    U16 measurementsPerSensor[NUMBER_OF_MEASUREMENTS_PER_SENSOR];
    for(int i = 0; i < NUMBER_OF_MEASUREMENTS_PER_SENSOR; i++)
    {
        trigger(sensorAlignment);
        U32 duration = pulseIn(sensorAlignment);
        U16 distance = ceil((duration/2)*SOUND_SPEED);
        measurementsPerSensor[i] = distance;
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_BETWEEN_MEASUREMENTS));
    }
    measurements.at(sensorAlignment) = mostFrequent(measurementsPerSensor);
}

U32 DistanceSensors::pulseIn(const SensorAlignment &sensorAlignment)
{
    U8 pin = DISTANCE_SENSORS_PINS.find(sensorAlignment)->second.echoPin;

    auto timeUntilTimeout = std::chrono::high_resolution_clock::now();
    while(digitalRead(pin) == LOW)
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - timeUntilTimeout;
        if(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
            > SENSOR_TIMEOUT)
        {
            return 0;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    while(digitalRead(pin) == HIGH)
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        if(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
            > SENSOR_TIMEOUT)
        {
            return -1;
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

U16 DistanceSensors::mostFrequent(const U16 array[])
{
    // Insert all elements in hash.
    std::unordered_map<U16, U8> hashMap;
    for (U8 i = 0; i < NUMBER_OF_MEASUREMENTS_PER_SENSOR; i++)
    {
        hashMap[array[i]]++;
    }

    // find the max frequency
    U16 maxCount = 0;
    U16 mostFrequent = -1;
    for (auto element : hashMap)
    {
        if (maxCount < element.second)
        {
            mostFrequent = element.first;
            maxCount = element.second;
        }
    }
    return mostFrequent;
}

void DistanceSensors::trigger(const SensorAlignment &sensorAlignment)
{
    U8 triggerPin = DISTANCE_SENSORS_PINS.find(sensorAlignment)->second.triggerPin;
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(TRIGGER_DURATION);
    digitalWrite(triggerPin, LOW);
}
