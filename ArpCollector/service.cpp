#include "service.h"

Service::Service(QString location_name, QObject *parent) : QObject(parent) {
    this->location_name = location_name;
    this->connection = new ServerConnection(this);
    this->sniffer = new Sniffer(this);

    updateTimer = new QTimer(this);
    updateTimer->setInterval(500);
    updateTimer->setSingleShot(true);

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
    this->connection->send(this->location_name, this->sniffer->state);
}

void Service::onDataSent() {
    this->updateTimer->start();
}
