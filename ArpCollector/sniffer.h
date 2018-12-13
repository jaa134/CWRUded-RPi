#ifndef SNIFFER_H
#define SNIFFER_H

#include <QDebug>
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
        int numConnections;
        QDateTime timeStamp;
    };

    explicit Sniffer(QObject *parent = nullptr);
    State state;
    void update();

private:
    QList<Client> connections;
    const QString fileName = "arp-sync.txt";
    void parseSystemCall(QString);
    bool isValidMacAddress(QString);
    bool isValidIpAddress(QString);

signals:
    void updated();
    void errored(QString);
};

#endif // SNIFFER_H
