#ifndef SERVICE_H
#define SERVICE_H

#include "serverconnection.h"
#include "sniffer.h"
#include <QObject>
#include <QTimer>

class Service : public QObject
{
    Q_OBJECT

public:
    explicit Service(QString uri, QString location_name, int avgDeviceCount, QObject *parent = nullptr);
    void start();

private:
    QTimer *updateTimer;
    ServerConnection *connection;
    Sniffer *sniffer;

private slots:
    void onTimeout();
    void onSnifferUpdated();
    void onDataSent();
};

#endif // SERVICE_H
