#ifndef NTFYSERVICE_HPP_
#define NTFYSERVICE_HPP_

#include <QObject>
#include "../msg/Connection.hpp"

// Forward declaration of system classes
namespace bb { namespace system { class InvokeManager; class InvokeRequest; } }

class NtfyService : public QObject
{
    Q_OBJECT
public:
    NtfyService();
    virtual ~NtfyService() {}

private slots:
    void handleInvoke(const bb::system::InvokeRequest&);

private:
    bb::system::InvokeManager* m_invokeManager;
    msg::Connection* m_connection;
};

#endif
