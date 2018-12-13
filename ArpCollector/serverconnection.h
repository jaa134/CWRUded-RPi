#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "sniffer.h"
#include "logger.h"
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantMap>
#include <QJsonDocument>

class ServerConnection : public QObject
{
    Q_OBJECT

    struct Location {
        QString location_name;
        int location_type;
        int extent;
        QDateTime timestamp;
    };

public:
    explicit ServerConnection(QObject *parent = nullptr);
    void send(QString uri, QString location_name, Sniffer::State state);

private:
    QNetworkAccessManager *manager;

signals:
    void sent();
    void errored(QString);

private slots:
    void onReply(QNetworkReply* reply);
};

#endif // SERVERCONNECTION_H

