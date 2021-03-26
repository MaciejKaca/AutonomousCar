#pragma once

#include "common.h"
#include <QFile>

class FileHandling
{
public:
    FileHandling();
    ~FileHandling();

    U8 getBrakeButtonId();
    U8 getLeftBlinkerButtonId();
    U8 getRightBlinkerButtonId();
    U8 getExitButtonId();
    float getAccelBiasX();
    float getAccelBiasY();
    float getAccelBiasZ();
    float getGyroBiasX();
    float getGyroBiasY();
    float getGyroBiasZ();
    float getMagBiasX();
    float getMagBiasY();
    float getMagBiasZ();
    float getMagScaleX();
    float getMagScaleY();
    float getMagScaleZ();

    void setBrakeButtonId(const S32& value);
    void setLeftBlinkerButtonId(const S32& value);
    void setRightBlinkerButtonId(const S32& value);
    void setExitButtonId(const S32& value);
    void setAccelBiasX(const float& value);
    void setAccelBiasY(const float& value);
    void setAccelBiasZ(const float& value);
    void setGyroBiasX(const float& value);
    void setGyroBiasY(const float& value);
    void setGyroBiasZ(const float& value);
    void setMagBiasX(const float& value);
    void setMagBiasY(const float& value);
    void setMagBiasZ(const float& value);
    void setMagScaleX(const float& value);
    void setMagScaleY(const float& value);
    void setMagScaleZ(const float& value);

private:
    const QString GAMEPAD_SETTINGS_FILE_NAME = "gamepadSettings.txt";
    const QString CALIBRATION_FILE_NAME = "calibration.txt";
    const QString FILE_PATH = "/home/$USER";
    const QString BRAKE_BUTTON_NAME = "BrakeButtonId";
    const QString LEFT_BLINKER_NAME = "LeftBlinkerButtonId";
    const QString RIGHT_BLINKER_NAME = "RightBlinkerButtonId";
    const QString EXIT_BUTTON_NAME = "ExitButtonId";
    const QString ACCEL_BIAS_X = "AcelBiasX";
    const QString ACCEL_BIAS_Y = "AcelBiasY";
    const QString ACCEL_BIAS_Z = "AcelBiasZ";
    const QString GYR_BIAS_X = "GyroBiasX";
    const QString GYR_BIAS_Y = "GyroBiasY";
    const QString GYR_BIAS_Z = "GyroBiasZ";
    const QString MAG_BIAS_X = "MagBiasX";
    const QString MAG_BIAS_Y = "MagBiasY";
    const QString MAG_BIAS_Z = "MagBiasZ";
    const QString MAG_SCALE_X = "MagScaleX";
    const QString MAG_SCALE_Y = "MagScaleY";
    const QString MAG_SCALE_Z = "MagScaleZ";
    const QString END_OF_LINE = "\n";

    QFile gamepadSettingsFile;
    QFile calibrationSettingsFile;
    std::mutex file_mutex;

    void saveToFile(const QString& attributeName, const S32& value);
    void saveToFile(const QString& attributeName, const float& value);
    void readFromFile(const QString& attributeName, S32& value);
    void readFromFile(const QString& attributeName, float& value);
};
