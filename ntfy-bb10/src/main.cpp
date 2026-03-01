#include <bb/cascades/Application>
#include <QLocale>
#include <QTranslator>
#include "applicationui.hpp"

#include <QtSql/QtSql>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // This is the line that was failing - we pass &app here
    ApplicationUI appui(&app);

    return Application::exec();
}
