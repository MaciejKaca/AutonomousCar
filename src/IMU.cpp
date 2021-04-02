#include <inc/IMU.h>

#include <QApplication>
#include <QVector3D>
#include <Forms/calibrationwindow.h>

IMU::IMU(SerialPort* _serialPort,  FileHandling* _fileSettings) : serialPort(_serialPort), fileSettings(_fileSettings)
{
    calibrationWasPerformed = false;
    collectMeasurements = false;

    accelBias = new QVector3D();
    gyroBias = new QVector3D();
    magBias = new QVector3D();
    magScale = new QVector3D();
}

IMU::~IMU()
{
    stopMeasurements();
    delete accelBias;
    delete gyroBias;
    delete magBias;
    delete magScale;
}

void IMU::calibrate()
{
    calibrationWasPerformed = true;

    calibrationWindow window;
    window.changeCalibrationType(calibrationWindow::CalibrationType::ACCEL_GYRO_CALIBRATION);
    window.show();

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
        window.changeCalibrationType(calibrationWindow::CalibrationType::MAG_CALIBRATION);
        window.show();
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
        accelBias->setX(calibrationCfm.accelrBias.x);
        accelBias->setY(calibrationCfm.accelrBias.y);
        accelBias->setZ(calibrationCfm.accelrBias.z);
        gyroBias->setX(calibrationCfm.gyroBias.x);
        gyroBias->setY(calibrationCfm.gyroBias.y);
        gyroBias->setZ(calibrationCfm.gyroBias.z);
        magBias->setX(calibrationCfm.magBias.x);
        magBias->setY(calibrationCfm.magBias.y);
        magBias->setZ(calibrationCfm.magBias.z);
        magScale->setX(calibrationCfm.magScale.x);
        magScale->setY(calibrationCfm.magScale.y);
        magScale->setZ(calibrationCfm.magScale.z);
        saveCalibration();
        window.changeCalibrationType(calibrationWindow::CalibrationType::CALIBRATION_DONE);
        window.show();
    }
    else
    {
        qCritical("in IMU::calibrate, failed to calibrate Accelerometer and Gyrometer");
        return;
    }
}

void IMU::receiveMeasurements()
{
    readCalibration();
    ConfigurationReq configurationReq;
    configurationReq.mpuSettings = mpuSettings;
    configurationReq.accelrBias.x = accelBias->x();
    configurationReq.accelrBias.y = accelBias->y();
    configurationReq.accelrBias.z = accelBias->z();
    configurationReq.gyroBias.x = gyroBias->x();
    configurationReq.gyroBias.y = gyroBias->y();
    configurationReq.gyroBias.z = gyroBias->z();
    configurationReq.magBias.x = magBias->x();
    configurationReq.magBias.y = magBias->y();
    configurationReq.magBias.z = magBias->z();
    configurationReq.magScale.x = magScale->x();
    configurationReq.magScale.y = magScale->y();
    configurationReq.magScale.z = magScale->z();
    serialPort->send((U8*)&configurationReq, sizeof(ConfigurationReq));

    while(collectMeasurements)
    {
        MeasurementInd measurementInd;
        while(serialPort->read((U8*)&measurementInd, sizeof(MeasurementInd)))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
        }
        std::lock_guard<std::mutex> lock(savingMeasurements_mutex);
        roll = measurementInd.roll;
        roll = measurementInd.pitch;
        roll = measurementInd.yaw;
    }
}

void IMU::readCalibration()
{
    accelBias->setX(fileSettings->getAccelBiasX());
    accelBias->setY(fileSettings->getAccelBiasY());
    accelBias->setZ(fileSettings->getAccelBiasZ());
    gyroBias->setX(fileSettings->getGyroBiasX());
    gyroBias->setY(fileSettings->getGyroBiasY());
    gyroBias->setZ(fileSettings->getGyroBiasZ());
    magBias->setX(fileSettings->getMagBiasX());
    magBias->setY(fileSettings->getMagBiasY());
    magBias->setZ(fileSettings->getMagBiasZ());
    magScale->setX(fileSettings->getMagScaleX());
    magScale->setY(fileSettings->getMagScaleY());
    magScale->setZ(fileSettings->getMagScaleZ());
}

void IMU::saveCalibration()
{
    fileSettings->setAccelBiasX(accelBias->x());
    fileSettings->setAccelBiasY(accelBias->y());
    fileSettings->setAccelBiasZ(accelBias->z());
    fileSettings->setGyroBiasX(gyroBias->x());
    fileSettings->setGyroBiasY(gyroBias->y());
    fileSettings->setGyroBiasZ(gyroBias->z());
    fileSettings->setMagBiasX(magBias->x());
    fileSettings->setMagBiasY(magBias->y());
    fileSettings->setMagBiasZ(magBias->z());
    fileSettings->setMagScaleX(magScale->x());
    fileSettings->setMagScaleY(magScale->y());
    fileSettings->setMagScaleZ(magScale->z());
}

void IMU::startCalibration()
{
    imuThread = std::async(std::launch::async, &IMU::calibrate, this);
}

void IMU::startMeasurements()
{
    if(calibrationWasPerformed)
    {
        imuThread.get();
        calibrationWasPerformed = false;
    }

    collectMeasurements= true;
    imuThread = std::async(std::launch::async, &IMU::receiveMeasurements, this);
}


void IMU::stopMeasurements()
{
    collectMeasurements = false;
    auto status = imuThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        imuThread.get();
    }
}
