#include "sniffer.h"

Sniffer::Sniffer(int avgDeviceCount, QObject *parent)
    : QObject(parent)
    , avgDeviceCount(avgDeviceCount)
{
    state.extent = "Unknown";
    state.timeStamp = QDateTime::currentDateTimeUtc();
}

void Sniffer::update() {
    system(qPrintable("arp-scan -l -g -q --retry 5 > " + fileName));

    QFile f(fileName);
    if (f.open(QIODevice::ReadOnly)) {
        QString data = f.readAll();
        f.close();

        if (data.isEmpty()) {
            qDebug(logCritical()) << "Failed System Call";
            emit errored("Failed System Call");
            return;
        }

        parseSystemCall(data);
        determineState();
        qDebug(logInfo()) << "Sniffer Updated...";
        emit updated();
    }
    else {
        qDebug(logCritical()) << "File IO";
        emit errored("File IO");
    }
}

void Sniffer::parseSystemCall(QString data) {
    connections.clear();
    QStringList split = data.replace('\n', ' ').replace('\t', ' ').split(' ');
    for (int i = 0; i < split.length() - 1; i++) {
        if (isValidIpAddress(split.at(i)) && isValidMacAddress(split.at(i + 1))) {
            Client c;
            c.ip = split.at(i);
            c.mac = split.at(i + 1).toUpper();
            connections.append(c);
            i++;
        }
    }
}

bool Sniffer::isValidIpAddress(QString ip) {
    QRegExp regExMacAddress("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
    return regExMacAddress.exactMatch(ip);
}

bool Sniffer::isValidMacAddress(QString mac) {
    QRegExp regExMacAddress("([0-9A-F]{2}[:-]){5}([0-9A-F]{2})");
    return regExMacAddress.exactMatch(mac.toUpper());
}

void Sniffer::determineState() {
    state.timeStamp = QDateTime::currentDateTime();

    int numDevicesFound = connections.length();
    double ratio = double(numDevicesFound) / double(avgDeviceCount);

    if (ratio > 0 && ratio <= 0.4)
        state.extent = "Empty";
    if (ratio > 0.4 && ratio <= 0.8)
        state.extent = "Light";
    if (ratio > 0.8 && ratio <= 1.2)
        state.extent = "Moderate";
    if (ratio > 1.2 && ratio <= 1.6)
        state.extent = "Heavy";
    if (ratio > 1.6)
        state.extent = "Extreme";
}
