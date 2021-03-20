#include <inc/FileHandling.h>
#include <stdlib.h>
#include <qfile.h>
#include <QDir>

FileHandling::FileHandling()
{
    settingsFile.setFileName(SETTINGS_FILE_NAME);
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
    return static_cast<U8>(readFromFile(EXIT_BUTTON_NAME));
}

U8 FileHandling::getBrakeButtonId()
{
    return static_cast<U8>(readFromFile(BRAKE_BUTTON_NAME));
}

U8 FileHandling::getLeftBlinkerButtonId()
{
    return static_cast<U8>(readFromFile(LEFT_BLINKER_NAME));
}

U8 FileHandling::getRightBlinkerButtonId()
{
    return static_cast<U8>(readFromFile(RIGHT_BLINKER_NAME));
}

void FileHandling::saveToFile(const QString& attributeName, const S32& value)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(settingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
       std::vector<QString> fileLines;

       while (!settingsFile.atEnd())
       {
            QString line = settingsFile.readLine();

            if(line.left(attributeName.length()) != attributeName)
            {
                 fileLines.push_back(line);
            }
       }

       qInfo() << "in FileHandling::saveToFile, saving attribute=" <<
                     attributeName << " value=" << value;
       QString newAttribute = attributeName + QString::number(value) + END_OF_LINE;
       fileLines.push_back(newAttribute);

       settingsFile.resize(0);
       QTextStream out(&settingsFile);
       for(auto& line : fileLines)
       {
           out << line;
       }
       settingsFile.close();
    }
    else
    {
        qWarning("in FileHandling::saveToFile, failed to open file");
    }
}

S32 FileHandling::readFromFile(const QString& attributeName)
{
    std::lock_guard<std::mutex> lock(file_mutex);
    if(settingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        while (!settingsFile.atEnd())
        {
            QString line = settingsFile.readLine();

            if(line.left(attributeName.length()) == attributeName)
            {
                QString attributeValue = QString::fromStdString(line.toStdString().
                                                                substr(attributeName.length(),
                                                                       line.toStdString().length()));
                settingsFile.close();
                return attributeValue.toLong();
            }
        }
        settingsFile.close();
        return INT32_MIN;
    }
    else
    {
        qWarning("in FileHandling::readFromFile, failed to open file");
        return INT32_MIN;
    }
}
