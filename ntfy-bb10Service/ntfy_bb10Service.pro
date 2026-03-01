APP_NAME = ntfy_bb10Service

# 'cascades' is required for the bb::system and bb::data headers
QT += network sql cascades
CONFIG += qt warn_on

include(config.pri)

# Link against the necessary libraries
LIBS += -lbb -lbbsystem -lbbplatform -lbbdata

# Ensure the compiler looks in the src directory for our local headers
INCLUDEPATH += src