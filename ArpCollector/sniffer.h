#ifndef SNIFFER_H
#define SNIFFER_H

#include "logger.h"
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QList>

class Sniffer : public QObject
{
    Q_OBJECT

    struct Client {
        QString ip;
        QString mac;
    };

public:

    struct State {
        QDateTime timeStamp;
        QString extent;
    };

    explicit Sniffer(int avgDeviceCount, QObject *parent = nullptr);
    State state;
    void update();

private:
    const int avgDeviceCount;
    QList<Client> connections;
    const QString fileName = "arp-sync.txt";
    void parseSystemCall(QString);
    bool isValidMacAddress(QString);
    bool isValidIpAddress(QString);
    void determineState();

signals:
    void updated();
    void errored(QString);
};

#endif // SNIFFER_H
