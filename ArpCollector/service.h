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
    explicit Service(QString location_name, QObject *parent = nullptr);
    void start();

private:
    QString location_name;
    QTimer *updateTimer;
    ServerConnection *connection;
    Sniffer *sniffer;

private slots:
    void onTimeout();
    void onSnifferUpdated();
    void onDataSent();
};

#endif // SERVICE_H
