#include "NtfyService.hpp"
#include "../db/DatabaseHelper.hpp"
#include <bb/system/InvokeManager>
#include <QtSql/QtSql>
#include <QDebug>

NtfyService::NtfyService() :
        QObject(),
        m_invokeManager(new bb::system::InvokeManager(this)),
        m_connection(new msg::Connection(this))
{
    // Listen for incoming invocation requests from the UI
    bool ok = connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                      this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));

    qDebug() << "ntfy Service: Started. Connection to InvokeManager:" << ok;
}

void NtfyService::handleInvoke(const bb::system::InvokeRequest& request)
{
    qDebug() << "ntfy Service: Invoked with action" << request.action();

    // Reset existing streams to prevent duplicate listeners
    m_connection->disconnectAll();

    // Load all topics from the shared database
    QSqlDatabase db = db::DatabaseHelper::getDatabase();
    QSqlQuery query("SELECT baseUrl, topic FROM subscription", db);

    while (query.next()) {
        QString server = query.value(0).toString();
        QString topic = query.value(1).toString();

        qDebug() << "ntfy Service: Connecting to" << server << "/" << topic;
        m_connection->connectToTopic(server, topic);
    }
}
