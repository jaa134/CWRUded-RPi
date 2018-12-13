#include "serverconnection.h"

ServerConnection::ServerConnection(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void ServerConnection::send(QString location_name, Sniffer::State state) {
    QVariantMap mapData;
    mapData.insert("location_name", location_name);
    mapData.insert("extent", state.numConnections);
    QJsonDocument jsonData = QJsonDocument::fromVariant(mapData);

    QNetworkAccessManager manager;

    QNetworkRequest request(QUrl("http://localhost:5000/api/updateLocation"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    /* QNetworkReply* reply = */ manager.put(request, jsonData.toJson());
    qDebug() << jsonData;
    emit sent();
}
