#include <QCoreApplication>
#include "service/NtfyService.hpp"

/**
 * Entry point for the Headless Service.
 * Using QCoreApplication because a service has no UI/Cascades elements.
 */
Q_DECL_EXPORT int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // Initialize our service class
    NtfyService ntfyService;

    // Enter the main event loop
    return app.exec();
}
