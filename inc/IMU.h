#pragma once

#include "common.h"
#include "serialport.h"
#include "FileHandling.h"

class IMU
{
public:
    IMU(SerialPort* _serialPort, FileHandling* fileSettings);
    ~IMU();
    void startCalibration();
    void startMeasurements();
    void stopMeasurements();

    const S16& getPitch();
    const S16& getRoll();
    const S16& getYaw();

private:
    S16 roll;
    S16 pitch;
    S16 yaw;
    MPU9250Setting mpuSettings;
    const float magneticDelication = 4.5;

    SerialPort* const serialPort;
    FileHandling* const fileSettings;

    //MPU9250Setting readSettings();
    void readCalibration();
    void saveCalibration();
    QVector3D* accelBias;
    QVector3D* gyroBias;
    QVector3D* magBias;
    QVector3D* magScale;

    void receiveMeasurements();
    void calibrate();

    const U16 REFRESH_TIME = 1; //milliseconds

    std::future<void> imuThread;
    std::mutex savingMeasurements_mutex;

    bool calibrationWasPerformed;
    bool collectMeasurements;
};
