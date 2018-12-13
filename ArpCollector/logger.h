#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QScopedPointer>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

class Logger : public QObject
{
    Q_OBJECT

public:
    static void setup();

private:
    static const QString outFolder;
    static QScopedPointer<QFile> m_logFile;
    static QString makeFileName();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
