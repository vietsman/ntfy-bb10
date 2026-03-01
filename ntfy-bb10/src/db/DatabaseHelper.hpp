#ifndef DATABASEHELPER_HPP_
#define DATABASEHELPER_HPP_

#include <QtSql/QtSql>
#include <QDir>
#include <QDebug>

namespace db {
class DatabaseHelper {
public:
    static QSqlDatabase getDatabase() {
        QSqlDatabase db = QSqlDatabase::database("ntfy_db");
        if (!db.isValid()) {
            db = QSqlDatabase::addDatabase("QSQLITE", "ntfy_db");
            // Points to /accounts/1000/appdata/ca.vietsman.ntfy/data/ntfy.db
            // This is the only shared sandbox folder guaranteed to work.
            QString path = QDir::currentPath() + "/data/ntfy.db";
            db.setDatabaseName(path);
        }

        if (!db.isOpen()) {
            if (db.open()) {
                QSqlQuery query(db);
                query.exec("CREATE TABLE IF NOT EXISTS subscription (id INTEGER PRIMARY KEY AUTOINCREMENT, baseUrl TEXT, topic TEXT)");
                query.exec("CREATE TABLE IF NOT EXISTS message (id INTEGER PRIMARY KEY AUTOINCREMENT, topic TEXT, title TEXT, message TEXT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");
                // Force Write-Ahead Logging OFF to ensure UI sees Service writes immediately
                query.exec("PRAGMA journal_mode = DELETE");
                qDebug() << "ntfy DB: Ready at" << db.databaseName();
            }
        }
        return db;
    }
};
}
#endif
