#include <inc/common.h>
#include <inc/DistanceSensors.h>
#include <wiringPi.h>
#include <chrono>
#include <iostream>

DistanceSensors::DistanceSensors()
{
    qInfo("in DistanceSensors, initializing constructor");
    measurements.fill(0);
    terminateThread = false;
    pinMode(25, INPUT);
    pinMode(TRIGGER_SENSOR_PIN, OUTPUT);
    digitalWrite(TRIGGER_SENSOR_PIN, LOW);
}

DistanceSensors::~DistanceSensors()
{
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
        for(int i = 0; i < NUMBER_OF_SENSORS; i++)
        {
            makeMeasurement(i);
        }
    }
}

void DistanceSensors::makeMeasurement(const U8 &sensorAlignment)
{
    U16 measurementsPerSensor[NUMBER_OF_MEASUREMENTS_PER_SENSOR];
    for(int i = 0; i < NUMBER_OF_MEASUREMENTS_PER_SENSOR; i++)
    {
        trigger();
        U32 duration = pulseIn(sensorAlignment);
        U16 distance = ceil((duration/2)*SOUND_SPEED);
        measurementsPerSensor[i] = distance;
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_BETWEEN_MEASUREMENTS));
    }

    measurements.at(sensorAlignment) = mostFrequent(measurementsPerSensor);
}

U32 DistanceSensors::pulseIn(const U8 &sensorAlignment)
{
    U8 pin = DISTANCE_SENSORS_PINS[sensorAlignment];

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
    while(digitalRead(pin) == HIGH);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

U16 DistanceSensors::mostFrequent(U16 array[])
{
    // Insert all elements in hash.
    std::unordered_map<U16, U8> hashMap;
    for (U8 i = 0; i < NUMBER_OF_MEASUREMENTS_PER_SENSOR; i++)
    {
        hashMap[array[i]]++;
    }

    // find the max frequency
    U16 maxCount = 0;
    U8 mostFrequent = -1;
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

void DistanceSensors::trigger()
{
    digitalWrite(TRIGGER_SENSOR_PIN, HIGH);
    delayMicroseconds(TRIGGER_DELAY);
    digitalWrite(TRIGGER_SENSOR_PIN, LOW);
}
