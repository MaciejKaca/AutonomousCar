#include "inc/IMU.h"
#include <QApplication>
#include "Forms/accelgyrocalibrationwindow.h"

IMU::IMU(SerialPort* _serialPort) : serialPort(_serialPort)
{
}

void IMU::calibrate()
{
    CalibrationReq calibrationReq;
    calibrationReq.mpuSettings = mpuSettings;
    calibrationReq.magneticDelication = magneticDelication;
    serialPort->send((U8*)&calibrationReq, sizeof(calibrationReq));

    CalibrationCfm calibrationCfm;

    while(serialPort->read((U8*)&calibrationCfm, sizeof(calibrationCfm)))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
    }

    if(calibrationCfm.isValid)
    {
        AccelGyroCalibrationWindow accelGyroWindow;
        accelGyroWindow.show();
    }
    else
    {
        qCritical("in IMU::calibrate, failed to calibrate Accelerometer and Gyrometer");
        return;
    }

    while(serialPort->read((U8*)&calibrationCfm, sizeof(calibrationCfm)))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
    }

    if(calibrationCfm.isValid)
    {
        AccelGyroCalibrationWindow accelGyroWindow;
        accelGyroWindow.show();
    }
    else
    {
        qCritical("in IMU::calibrate, failed to calibrate Accelerometer and Gyrometer");
        return;
    }
}
