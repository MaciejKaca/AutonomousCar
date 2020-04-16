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

    pinMode(COMMON_ECHO, INPUT);
    for(int i=0; i<NUMBER_OF_ADDRESS_PINS; i++)
    {
        pinMode(MULTIPLEXER_ADDRESS_PINS[i], OUTPUT);
        digitalWrite(MULTIPLEXER_ADDRESS_PINS[i], LOW);
    }
    for(auto sensorPins : DISTANCE_SENSORS_PINS)
    {
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

            if(terminateThread)
            {
                break;
            }
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
        if(distance != 0)
        {
            measurementsPerSensor[i] = distance;
        }
        else
        {
            if(getDistance(sensorAlignment) <= DISTANCE_THRESHOLD)
            {
                measurementsPerSensor[i] = MIN_DISTANCE;
            }
            else
            {
                measurementsPerSensor[i] = MAX_DISTANCE;
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(DELAY_BETWEEN_MEASUREMENTS));
    }
    measurements.at(sensorAlignment) = mostFrequent(measurementsPerSensor);
}

U32 DistanceSensors::pulseIn(const SensorAlignment &sensorAlignment)
{
    U8 echoAddress = DISTANCE_SENSORS_PINS.find(sensorAlignment)->second.echoAddress;
    setMultiplexerAddress(echoAddress);

    auto timeUntilTimeout = std::chrono::steady_clock::now();
    while(digitalRead(COMMON_ECHO) == LOW)
    {
        auto elapsed = std::chrono::steady_clock::now() - timeUntilTimeout;
        if(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
            > SENSOR_TIMEOUT)
        {
            qCritical() << "in DistanceSensors::pulseIn, timeout on LOW state. "
                           "Sensor disconected. sensorAlignment=" << sensorAlignment;
            throw EXIT_BY_MISSING_DISTANCE_SENSOR;
        }
    }

    auto start = std::chrono::steady_clock::now();
    while(digitalRead(COMMON_ECHO) == HIGH)
    {
        auto elapsed = std::chrono::steady_clock::now() - start;
        if(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
            > SENSOR_TIMEOUT)
        {
            return 0;
        }
    }

    auto elapsed = std::chrono::steady_clock::now() - start;
    return static_cast<U32>(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count());
}

U16 DistanceSensors::mostFrequent(const U16 array[])
{
    // Insert all elements in hash.
    std::unordered_map<U16, U16> hashMap;
    for (U8 i = 0; i < NUMBER_OF_MEASUREMENTS_PER_SENSOR; i++)
    {
        hashMap[array[i]]++;
    }

    // find the max frequency
    U16 maxCount = 0;
    U16 mostFrequent = 0;
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
    std::this_thread::sleep_for(std::chrono::microseconds(TRIGGER_DURATION));
    digitalWrite(triggerPin, LOW);
}

void DistanceSensors::setMultiplexerAddress(const U8 &address)
{
    for(int i=0; i<NUMBER_OF_ADDRESS_PINS; i++)
    {
        bool state = address&static_cast<U8>(0x1 << i);
        digitalWrite(MULTIPLEXER_ADDRESS_PINS[i], state);
    }
}
