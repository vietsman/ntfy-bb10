#ifndef APPLICATIONUI_HPP_
#define APPLICATIONUI_HPP_

#include <QObject>
#include <bb/cascades/GroupDataModel>

namespace bb { namespace cascades { class Application; }}

class ApplicationUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasSubscriptions READ hasSubscriptions NOTIFY subscriptionsChanged)

public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

    Q_INVOKABLE void addSubscription(const QString& topic, const QString& server, bool isCustom);
    Q_INVOKABLE bb::cascades::GroupDataModel* getMessageModel(const QString& topic);
    Q_INVOKABLE void clearMessages(const QString& topic);

    bool hasSubscriptions() const { return !m_model->isEmpty(); }

signals:
    void subscriptionsChanged();

private slots:
    void loadSubscriptions();

private:
    bb::cascades::GroupDataModel* m_model;
};

#endif
