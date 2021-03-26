#include <inc/FileHandling.h>
#include <stdlib.h>
#include <qfile.h>
#include <QDir>

FileHandling::FileHandling()
{
    gamepadSettingsFile.setFileName(GAMEPAD_SETTINGS_FILE_NAME);
    calibrationSettingsFile.setFileName(CALIBRATION_FILE_NAME);
    QDir::setCurrent(FILE_PATH);
}

FileHandling::~FileHandling()
{
}

void FileHandling::setExitButtonId(const S32& value)
{
    saveToFile(EXIT_BUTTON_NAME, value);
}

void FileHandling::setBrakeButtonId(const S32 &value)
{
    saveToFile(BRAKE_BUTTON_NAME, value);
}

void FileHandling::setLeftBlinkerButtonId(const S32 &value)
{
    saveToFile(LEFT_BLINKER_NAME, value);
}

void FileHandling::setRightBlinkerButtonId(const S32 &value)
{
    saveToFile(RIGHT_BLINKER_NAME, value);
}

U8 FileHandling::getExitButtonId()
{
    S32 value = 0;
    readFromFile(EXIT_BUTTON_NAME, value);
    return static_cast<U8>(value);
}

U8 FileHandling::getBrakeButtonId()
{
    S32 value = 0;
    readFromFile(BRAKE_BUTTON_NAME, value);
    return static_cast<U8>(value);
}

U8 FileHandling::getLeftBlinkerButtonId()
{
    S32 value = 0;
    readFromFile(LEFT_BLINKER_NAME, value);
    return static_cast<U8>(value);
}

U8 FileHandling::getRightBlinkerButtonId()
{
    S32 value = 0;
    readFromFile(RIGHT_BLINKER_NAME, value);
    return static_cast<U8>(value);
}

void FileHandling::setAccelBiasX(const float& value) {saveToFile(ACCEL_BIAS_X, value);}
void FileHandling::setAccelBiasY(const float& value) {saveToFile(ACCEL_BIAS_Y, value);}
void FileHandling::setAccelBiasZ(const float& value) {saveToFile(ACCEL_BIAS_Z, value);}
void FileHandling::setGyroBiasX(const float& value) {saveToFile(GYR_BIAS_X, value);}
void FileHandling::setGyroBiasY(const float& value) {saveToFile(GYR_BIAS_Y, value);}
void FileHandling::setGyroBiasZ(const float& value) {saveToFile(GYR_BIAS_Z, value);}
void FileHandling::setMagBiasX(const float& value) {saveToFile(MAG_BIAS_X, value);}
void FileHandling::setMagBiasY(const float& value) {saveToFile(MAG_BIAS_Y, value);}
void FileHandling::setMagBiasZ(const float& value) {saveToFile(MAG_BIAS_Z, value);}
void FileHandling::setMagScaleX(const float& value) {saveToFile(MAG_SCALE_X, value);}
void FileHandling::setMagScaleY(const float& value) {saveToFile(MAG_SCALE_Y, value);}
void FileHandling::setMagScaleZ(const float& value) {saveToFile(MAG_SCALE_Z, value);}

float FileHandling::getAccelBiasX()
{
    float value = 0;
    readFromFile(ACCEL_BIAS_X, value);
    return value;
}

float FileHandling::getAccelBiasY()
{
    float value = 0;
    readFromFile(ACCEL_BIAS_Y, value);
    return value;
}

float FileHandling::getAccelBiasZ()
{
    float value = 0;
    readFromFile(ACCEL_BIAS_Z, value);
    return value;
}

float FileHandling::getGyroBiasX()
{
    float value = 0;
    readFromFile(GYR_BIAS_X, value);
    return value;
}

float FileHandling::getGyroBiasY()
{
    float value = 0;
    readFromFile(GYR_BIAS_Y, value);
    return value;
}

float FileHandling::getGyroBiasZ()
{
    float value = 0;
    readFromFile(GYR_BIAS_Z, value);
    return value;
}

float FileHandling::getMagBiasX()
{
    float value = 0;
    readFromFile(MAG_BIAS_X, value);
    return value;
}

float FileHandling::getMagBiasY()
{
    float value = 0;
    readFromFile(MAG_BIAS_Y, value);
    return value;
}

float FileHandling::getMagBiasZ()
{
    float value = 0;
    readFromFile(MAG_BIAS_Z, value);
    return value;
}

float FileHandling::getMagScaleX()
{
    float value = 0;
    readFromFile(MAG_SCALE_X, value);
    return value;
}

float FileHandling::getMagScaleY()
{
    float value = 0;
    readFromFile(MAG_SCALE_Y, value);
    return value;
}

float FileHandling::getMagScaleZ()
{
    float value = 0;
    readFromFile(MAG_SCALE_Z, value);
    return value;
}


void FileHandling::saveToFile(const QString& attributeName, const S32& value)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(gamepadSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
       std::vector<QString> fileLines;

       while (!gamepadSettingsFile.atEnd())
       {
            QString line = gamepadSettingsFile.readLine();

            if(line.left(attributeName.length()) != attributeName)
            {
                 fileLines.push_back(line);
            }
       }

       qInfo() << "in FileHandling::saveToFile, saving attribute=" <<
                     attributeName << " value=" << value;
       QString newAttribute = attributeName + QString::number(value) + END_OF_LINE;
       fileLines.push_back(newAttribute);

       gamepadSettingsFile.resize(0);
       QTextStream out(&gamepadSettingsFile);
       for(auto& line : fileLines)
       {
           out << line;
       }
       gamepadSettingsFile.close();
    }
    else
    {
        qWarning("in FileHandling::saveToFile, failed to open file");
    }
}

void FileHandling::saveToFile(const QString& attributeName, const float& value)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(calibrationSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
       std::vector<QString> fileLines;

       while (!calibrationSettingsFile.atEnd())
       {
            QString line = calibrationSettingsFile.readLine();

            if(line.left(attributeName.length()) != attributeName)
            {
                 fileLines.push_back(line);
            }
       }

       qInfo() << "in FileHandling::saveToFile, saving attribute=" <<
                     attributeName << " value=" << value;
       QString newAttribute = attributeName + QString::number((double)value) + END_OF_LINE;
       fileLines.push_back(newAttribute);

       calibrationSettingsFile.resize(0);
       QTextStream out(&calibrationSettingsFile);
       for(auto& line : fileLines)
       {
           out << line;
       }
       calibrationSettingsFile.close();
    }
    else
    {
        qWarning("in FileHandling::saveToFile, failed to open file");
    }
}

void FileHandling::readFromFile(const QString& attributeName, S32& value)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(gamepadSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        while (!gamepadSettingsFile.atEnd())
        {
            QString line = gamepadSettingsFile.readLine();

            if(line.left(attributeName.length()) == attributeName)
            {
                QString attributeValue = QString::fromStdString(line.toStdString().
                                                                substr(attributeName.length(),
                                                                line.toStdString().length()));
                gamepadSettingsFile.close();
                value = attributeValue.toLong();
            }
        }
        gamepadSettingsFile.close();
        qWarning("in FileHandling::readFromFile, attribute %s not found", qUtf8Printable(attributeName));
    }
    else
    {
        qWarning("in FileHandling::readFromFile, failed to open file");
    }
}

void FileHandling::readFromFile(const QString& attributeName, float& value)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(calibrationSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        while (!calibrationSettingsFile.atEnd())
        {
            QString line = calibrationSettingsFile.readLine();

            if(line.left(attributeName.length()) == attributeName)
            {
                QString attributeValue = QString::fromStdString(line.toStdString().
                                                                substr(attributeName.length(),
                                                                line.toStdString().length()));
                calibrationSettingsFile.close();
                value = attributeValue.toFloat();
            }
        }
        calibrationSettingsFile.close();
        qWarning("in FileHandling::readFromFile, attribute %s not found", qUtf8Printable(attributeName));
    }
    else
    {
        qWarning("in FileHandling::readFromFile, failed to open file");
    }
}
