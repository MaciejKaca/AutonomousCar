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

    void setBrakeButtonId(const S32& value);
    void setLeftBlinkerButtonId(const S32& value);
    void setRightBlinkerButtonId(const S32& value);
    void setExitButtonId(const S32& value);

private:
    const QString SETTINGS_FILE_NAME = "settings.txt";
    const QString FILE_PATH = "/home/$USER";
    const QString BRAKE_BUTTON_NAME = "BrakeButtonId";
    const QString LEFT_BLINKER_NAME = "LeftBlinkerButtonId";
    const QString RIGHT_BLINKER_NAME = "RightBlinkerButtonId";
    const QString EXIT_BUTTON_NAME = "ExitButtonId";
    const QString END_OF_LINE = "\n";

    QFile settingsFile;
    std::mutex file_mutex;

    void saveToFile(const QString& attributeName, const S32& value);
    S32 readFromFile(const QString& attributeName);
};
