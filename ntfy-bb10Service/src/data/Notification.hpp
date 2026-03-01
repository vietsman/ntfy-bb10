#ifndef NOTIFICATION_HPP_
#define NOTIFICATION_HPP_

#include <QObject>
#include <QString>

namespace data {

class Notification : public QObject {
    Q_OBJECT
public:
    explicit Notification(QObject* parent = 0)
        : QObject(parent), time(0), priority(3) {} // Initialized time here

    QString id;
    long long time;
    QString topic;
    QString message;
    QString title;
    int priority;
};

} /* namespace data */

#endif
