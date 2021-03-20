#pragma once

#define LOGSIZE 1024 * 100 //log size in bytes
#define LOGFILES 5

#include <QObject>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QTime>

namespace LOGUTILS
{
const QString logFolderName = "logs";

bool initLogging(bool suppressWarnings);
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);
static bool SUPPRESS_WARNINGS;
}
