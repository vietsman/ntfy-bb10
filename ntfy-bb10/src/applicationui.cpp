#include "applicationui.hpp"
#include "db/DatabaseHelper.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/system/InvokeManager>
#include <QtSql/QtSql>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app),
        m_model(new GroupDataModel(QStringList() << "topic", this))
{
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_uiController", this);
    qml->setContextProperty("_subscriptionModel", m_model);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    app->setScene(root);

    loadSubscriptions();
}

void ApplicationUI::addSubscription(const QString& topic, const QString& server, bool isCustom) {
    QString cleanTopic = topic.trimmed();
    if (cleanTopic.isEmpty()) return;

    QString finalServer = isCustom ? server.trimmed() : "https://ntfy.sh";

    if (isCustom && !finalServer.isEmpty() && !finalServer.startsWith("http")) {
        finalServer.prepend("https://");
    }

    QSqlDatabase db = db::DatabaseHelper::getDatabase();
    QSqlQuery query(db);
    query.prepare("INSERT INTO subscription (baseUrl, topic) VALUES (:base, :topic)");
    query.bindValue(":base", finalServer);
    query.bindValue(":topic", cleanTopic);

    if (query.exec()) {
        loadSubscriptions();

        bb::system::InvokeManager invokeManager;
        bb::system::InvokeRequest request;
        request.setTarget("ca.vietsman.ntfy.service");
        request.setAction("bb.action.START");
        invokeManager.invoke(request);
    }
}

bb::cascades::GroupDataModel* ApplicationUI::getMessageModel(const QString& topic) {
    // We set 'this' as the parent so the model persists while the UI is active
    GroupDataModel* model = new GroupDataModel(QStringList() << "timestamp", this);
    model->setGrouping(ItemGrouping::None);
    model->setSortingKeys(QStringList() << "timestamp");
    model->setSortedAscending(false);

    QSqlDatabase db = db::DatabaseHelper::getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT title, message, timestamp FROM message WHERE topic = :topic");
    query.bindValue(":topic", topic);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap item;
            item["title"] = query.value(0).toString().isEmpty() ? "No Title" : query.value(0).toString();
            item["message"] = query.value(1).toString();
            item["timestamp"] = query.value(2).toString();
            model->insert(item);
        }
    } else {
        qDebug() << "ntfy UI: Database error fetching messages:" << query.lastError().text();
    }

    return model;
}

void ApplicationUI::clearMessages(const QString& topic) {
    QSqlDatabase db = db::DatabaseHelper::getDatabase();
    QSqlQuery query(db);
    query.prepare("DELETE FROM message WHERE topic = :topic");
    query.bindValue(":topic", topic);
    query.exec();
}

void ApplicationUI::loadSubscriptions() {
    m_model->clear();
    QSqlDatabase db = db::DatabaseHelper::getDatabase();
    QSqlQuery query("SELECT topic, baseUrl FROM subscription", db);

    while (query.next()) {
        QVariantMap item;
        item["topic"] = query.value(0).toString();
        item["baseUrl"] = query.value(1).toString();
        m_model->insert(item);
    }

    emit subscriptionsChanged();
}
