#include "sniffer.h"

Sniffer::Sniffer(QObject *parent) : QObject(parent) {
    state.numConnections = 0;
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
        qDebug(logInfo()) << "Sniffer Updated...";
        emit updated();
    }
    else {
        qDebug(logCritical()) << "File IO";
        emit errored("File IO");
    }
}

void Sniffer::parseSystemCall(QString data) {
    //remove previous system call data
    connections.clear();
    //remove invalid chars
    QStringList split = data.replace('\n', ' ').replace('\t', ' ').split(' ');
    //append each packet
    for (int i = 0; i < split.length() - 1; i++) {
        if (isValidIpAddress(split.at(i)) && isValidMacAddress(split.at(i + 1))) {
            Client c;
            c.ip = split.at(i);
            c.mac = split.at(i + 1).toUpper();
            connections.append(c);
            i++;
        }
    }

    state.numConnections = connections.length();
    state.timeStamp = QDateTime::currentDateTime();
}

bool Sniffer::isValidIpAddress(QString ip) {
    QRegExp regExMacAddress("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
    return regExMacAddress.exactMatch(ip);
}

bool Sniffer::isValidMacAddress(QString mac) {
    QRegExp regExMacAddress("([0-9A-F]{2}[:-]){5}([0-9A-F]{2})");
    return regExMacAddress.exactMatch(mac.toUpper());
}
