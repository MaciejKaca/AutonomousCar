#include <inc/common.h>
#include <inc/DistanceSensors.h>

#include <wiringPi.h>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>

DistanceSensors::DistanceSensors()
{
    qInfo("in DistanceSensors, initializing constructor");
    SensorState sensorState;
    sensorState.distance = MAX_DISTANCE;
    sensorState.sensorActive = false;
    for(U8 i = 0; i < NUMBER_OF_SENSORS; i++)
    {
        measurements[i].distance = sensorState.distance;
        measurements[i].sensorActive = sensorState.sensorActive;
    }
    isDistanceSensorThreadActive = false;

    pinMode(COMMON_ECHO, INPUT);
    for(U8 i=0; i<MULTIPLEXER_ADDRESS_PINS.size(); i++)
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
    if(sensorQueueCounter.size() > 0)
    {
        qWarning() << "in DistanceSensors::~DistanceSensors, not all sensors cleared: ";
        for(auto sensor : sensorQueueCounter)
        {
            qWarning() << "Sensor nr: " << sensor.first << " called times: " << sensor.second;
        }
    }
    stopThread();
}

DistanceSensorsBase::~DistanceSensorsBase(){}

U16 DistanceSensors::getDistance(SensorAlignment sensorAlignment)
{
    std::lock_guard<std::mutex> lock(measurements_mutex);
    return measurements[sensorAlignment].distance;
}

void DistanceSensors::startThread()
{
    qInfo("in DistanceSensors::startThread, launching thread");

    if (!isDistanceSensorThreadActive)
    {
        qInfo("in DistanceSensors::startThread, thread launched");
        isDistanceSensorThreadActive = true;
        measurementThread = std::async(std::launch::async, &DistanceSensors::collectMeasurements, this);
    }
}

void DistanceSensors::stopThread()
{
    qInfo("in DistanceSensors::stopThread, stopping thread");
    if(isDistanceSensorThreadActive)
    {
        isDistanceSensorThreadActive = false;
        measurementThread.get();
    }
    qInfo("in DistanceSensors::stopThread, thread stopped");
}

void DistanceSensors::collectMeasurements()
{
    qInfo("in DistanceSensors::collectMeasurements, succesfull thread launching");
    while(sensorQueueCounter.size() > 0 && isDistanceSensorThreadActive)
    {
        for(auto sensor : sensorQueueCounter)
        {
            SensorAlignment sensorAlignment = sensor.first;
            makeMeasurement(sensorAlignment);

            if(!isDistanceSensorThreadActive)
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

    for(int checkingSensor = 0; checkingSensor < NUMBER_OF_MEASUREMENTS_PER_SENSOR; checkingSensor++)
    {
        trigger(sensorAlignment);

        U32 duration = pulseIn(sensorAlignment);
        U16 distance = ceil((duration/2)*SOUND_SPEED);

        if(distance != 0)
        {
            measurementsPerSensor[checkingSensor] = distance;
        }
        else
        {
            if(getDistance(sensorAlignment) <= DISTANCE_THRESHOLD)
            {
                measurementsPerSensor[checkingSensor] = MIN_DISTANCE;
            }
            else
            {
                measurementsPerSensor[checkingSensor] = MAX_DISTANCE;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_BETWEEN_MEASUREMENTS));
    }

    U16 mostFrequentMeasurement = mostFrequent(measurementsPerSensor);
    std::lock_guard<std::mutex> lock(measurements_mutex);
    measurements[sensorAlignment].distance = mostFrequentMeasurement;
    measurements[sensorAlignment].sensorActive = true;
}

inline U32 DistanceSensors::pulseIn(const SensorAlignment &sensorAlignment)
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
            return SENSOR_TIMEOUT;
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
    for(U8 i=0; i<MULTIPLEXER_ADDRESS_PINS.size(); i++)
    {
        bool state = address&static_cast<U8>(0x1 << i);
        digitalWrite(MULTIPLEXER_ADDRESS_PINS[i], state);
    }
}

void DistanceSensors::startAndWaitUntilSensorIsActive(std::vector<SensorAlignment> sensorsAlignment)
{
    startThread();
    for(auto sensor : sensorsAlignment)
    {
        while(measurements[sensor].sensorActive == false)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_BETWEEN_MEASUREMENTS));
        }
    }
}

void DistanceSensors::addSensorToQueue(std::vector<SensorAlignment> sensorsAlignment)
{
    std::lock_guard<std::mutex> lock(sensorQueue_mutex);
    stopThread();
    for(auto sensor : sensorsAlignment)
    {
        qInfo() << "in DistanceSensors::addSensorToQueue, adding sensor to queue nr: " << sensor;
        sensorQueueCounter[sensor]++;
        qInfo() << "in DistanceSensors::addSensorToQueue, successful added sensor to queue nr: " << sensor;
    }
    startAndWaitUntilSensorIsActive(sensorsAlignment);
}

void DistanceSensors::removeSensorFromQueue(std::vector<SensorAlignment> sensorsAlignment)
{
    std::lock_guard<std::mutex> lock(sensorQueue_mutex);
    stopThread();
    for(auto sensor : sensorsAlignment)
    {
        if(sensorQueueCounter.find(sensor) != sensorQueueCounter.end())
        {
            qInfo() << "in DistanceSensors::removeSensorFromQueue, deacreasing sensor counter nr: " << sensor;
            measurements[sensor].sensorActive = false;
            sensorQueueCounter[sensor]--;
            if(sensorQueueCounter[sensor] <= 0)
            {
                qInfo() << "in DistanceSensors::removeSensorFromQueue, removing sensor from que nr: " << sensor;
                sensorQueueCounter.erase(sensorQueueCounter.find(sensor));
            }
        }
        else
        {
            qWarning() << "in DistanceSensors::removeSensorFromQueue, sensor not found nr: " << sensor;
        }
    }
    if(sensorQueueCounter.size() > 0)
    {
        startThread();
    }
}
