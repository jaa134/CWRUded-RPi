#include "service.h"

Service::Service(QString uri, QString location_name, int avgDeviceCount, QObject *parent)
    : QObject(parent)
{
    this->sniffer = new Sniffer(avgDeviceCount, this);
    this->connection = new ServerConnection(uri, location_name, this);

    this->updateTimer = new QTimer(this);
    this->updateTimer->setInterval(5000);
    this->updateTimer->setSingleShot(true);

    connect(this->updateTimer, &QTimer::timeout, this, &Service::onTimeout);
    connect(this->sniffer, &Sniffer::updated, this, &Service::onSnifferUpdated);
    connect(this->connection, &ServerConnection::sent, this, &Service::onDataSent);
}

void Service::start() {
    this->updateTimer->start();
}

void Service::onTimeout() {
    this->sniffer->update();
}

void Service::onSnifferUpdated() {
    this->connection->send(this->sniffer->state);
}

void Service::onDataSent() {
    this->updateTimer->start();
}
