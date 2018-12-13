#include "logger.h"

Q_LOGGING_CATEGORY(logDebug,    "Debug")
Q_LOGGING_CATEGORY(logInfo,     "Info")
Q_LOGGING_CATEGORY(logWarning,  "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")

const QString Logger::outFolder = "logs";
QScopedPointer<QFile> Logger::m_logFile;

void Logger::setup() {
    QDir().mkpath(outFolder);
    m_logFile.reset(new QFile(makeFileName()));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);
}

QString Logger::makeFileName() {
    QString timeStamp = QDateTime::currentDateTime().toString("yyMMdd-hhmmsszzz");
    return outFolder + "/" + timeStamp + ".txt";
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }

    out << context.category << ": " << msg << endl;
    out.flush();
}
