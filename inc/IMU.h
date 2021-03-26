#ifndef IMU_H
#define IMU_H

#include "common.h"
#include "serialport.h"

class IMU
{
public:
    IMU(SerialPort* _serialPort);
    void calibrate();
    void startMeasurements();

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

    MPU9250Setting readSettings();
    QVector3D accelrBias();
    QVector3D gyroBias();
    QVector3D magBias();
    QVector3D magScale();

    const U16 REFRESH_TIME = 1; //milliseconds
};

#endif // IMU_H
