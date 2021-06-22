#pragma once

#include "common.h"
#include "Base/FileHandlingBase.h"
#include <QFile>

class FileHandling : FileHandlingBase
{
public:
    FileHandling();
    ~FileHandling();

    U8 getBrakeButtonId() override;
    U8 getLeftBlinkerButtonId() override;
    U8 getRightBlinkerButtonId() override;
    U8 getExitButtonId() override;
    float getAccelBiasX() override;
    float getAccelBiasY() override;
    float getAccelBiasZ() override;
    float getGyroBiasX() override;
    float getGyroBiasY() override;
    float getGyroBiasZ() override;
    float getMagBiasX() override;
    float getMagBiasY() override;
    float getMagBiasZ() override;
    float getMagScaleX() override;
    float getMagScaleY() override;
    float getMagScaleZ() override;

    void setBrakeButtonId(const S32& value) override;
    void setLeftBlinkerButtonId(const S32& value) override;
    void setRightBlinkerButtonId(const S32& value) override;
    void setExitButtonId(const S32& value) override;
    void setAccelBiasX(const float& value) override;
    void setAccelBiasY(const float& value) override;
    void setAccelBiasZ(const float& value) override;
    void setGyroBiasX(const float& value) override;
    void setGyroBiasY(const float& value) override;
    void setGyroBiasZ(const float& value) override;
    void setMagBiasX(const float& value) override;
    void setMagBiasY(const float& value) override;
    void setMagBiasZ(const float& value) override;
    void setMagScaleX(const float& value) override;
    void setMagScaleY(const float& value) override;
    void setMagScaleZ(const float& value) override;

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
