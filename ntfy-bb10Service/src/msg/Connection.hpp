#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>

namespace msg {

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(QObject* parent = 0);
    virtual ~Connection() {}

    void connectToTopic(const QString& baseUrl, const QString& topic);
    void disconnectAll();

private slots:
    void onReadyRead();
    void onReplyFinished();
    void onNetworkError(QNetworkReply::NetworkError);
    void onSslErrors(const QList<QSslError>&); // This was missing

private:
    void triggerHubNotification(const QString& title, const QString& body, const QString& topic);
    QNetworkAccessManager* m_networkManager;
    QList<QNetworkReply*> m_activeStreams;
};

} /* namespace msg */

#endif
