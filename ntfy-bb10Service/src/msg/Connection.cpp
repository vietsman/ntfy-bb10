#include "Connection.hpp"
#include "../db/DatabaseHelper.hpp"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslCertificate>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <bb/platform/Notification>
#include <bb/data/JsonDataAccess>
#include <bb/system/InvokeRequest>
#include <QtSql/QtSql>

namespace msg {

Connection::Connection(QObject* parent) :
    QObject(parent),
    m_networkManager(new QNetworkAccessManager(this)) {}

void Connection::connectToTopic(const QString& baseUrl, const QString& topic) {
    QString server = baseUrl;
    if (!server.endsWith("/")) server.append("/");

    QUrl url(server + topic + "/json");
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "ntfy-bb10/1.0");

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    QFile certFile("app/native/assets/isrgrootx1.der");
    if (certFile.open(QIODevice::ReadOnly)) {
        QSslCertificate cert(&certFile, QSsl::Der);
        QList<QSslCertificate> caCerts = sslConfig.caCertificates();
        caCerts.append(cert);
        sslConfig.setCaCertificates(caCerts);

        // Changed from TlsV1_2 to SecureProtocols for better compatibility
        sslConfig.setProtocol(QSsl::SecureProtocols);

        request.setSslConfiguration(sslConfig);
        certFile.close();
    }

    QNetworkReply* reply = m_networkManager->get(request);

    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));

    m_activeStreams.append(reply);
}

void Connection::onSslErrors(const QList<QSslError>& errors) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        // Log specifically what is failing for debugging
        foreach (const QSslError &e, errors) {
            qDebug() << "ntfy Service SSL Error:" << e.errorString();
        }
        reply->ignoreSslErrors();
    }
}

void Connection::onReadyRead() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        bb::data::JsonDataAccess jda;
        while (reply->canReadLine()) {
            QByteArray line = reply->readLine().trimmed();
            if (!line.isEmpty()) {
                QVariantMap map = jda.loadFromBuffer(line).toMap();
                if (map["event"].toString() == "message") {
                    QString topic = map["topic"].toString();
                    QString title = map["title"].toString();
                    QString messageText = map["message"].toString();

                    QSqlDatabase db = db::DatabaseHelper::getDatabase();
                    QSqlQuery query(db);
                    query.prepare("INSERT INTO message (topic, title, message) VALUES (:topic, :title, :message)");
                    query.bindValue(":topic", topic);
                    query.bindValue(":title", title);
                    query.bindValue(":message", messageText);
                    query.exec();

                    triggerHubNotification(title, messageText, topic);
                }
            }
        }
    }
}

void Connection::triggerHubNotification(const QString& title, const QString& body, const QString& topic) {
    bb::platform::Notification* n = new bb::platform::Notification(this);
    n->setTitle(title.isEmpty() ? topic : title);
    n->setBody(body);

    bb::system::InvokeRequest request;
    request.setTarget("ca.vietsman.ntfy.ui");
    request.setAction("bb.action.OPEN");

    n->setInvokeRequest(request);
    n->notify();
}

void Connection::onReplyFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        m_activeStreams.removeAll(reply);
        reply->deleteLater();
    }
}

void Connection::onNetworkError(QNetworkReply::NetworkError error) {
    qDebug() << "ntfy Service Error:" << error;
}

void Connection::disconnectAll() {
    foreach (QNetworkReply* reply, m_activeStreams) {
        reply->abort();
        reply->deleteLater();
    }
    m_activeStreams.clear();
}

} /* namespace msg */
