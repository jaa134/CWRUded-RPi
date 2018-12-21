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
    explicit ServerConnection(QString uri, QString location_name, QObject *parent = nullptr);
    void send(Sniffer::State state);

private:
    const QString uri;
    const QString location_name;
    QNetworkAccessManager *manager;

signals:
    void sent();
    void errored(QString);

private slots:
    void onReply(QNetworkReply* reply);
};

#endif // SERVERCONNECTION_H

