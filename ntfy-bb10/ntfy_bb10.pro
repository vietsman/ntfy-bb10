APP_NAME = ntfy_bb10

CONFIG += qt warn_on cascades10

include(config.pri)

LIBS += -lbb -lbbsystem -lbbsystem -lbbdata

QT += network sql cascades

# Ensure it looks in src
INCLUDEPATH += src