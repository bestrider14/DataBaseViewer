#include "databaseconnection.h"
#include <QHash>
#include <QSqlDatabase>

DatabaseConnection::DatabaseConnection() {}

QString DatabaseConnection::displayName(const QString &driver)
{
    static const QHash<QString, QString> names = {
                                                    { "QPSQL",   "PostgreSQL" },
                                                    { "QSQLITE", "SQLite"     },
                                                    { "QMYSQL",  "MySQL / MariaDB" },
                                                    { "QIBASE", "InterBase / Firebird" },
                                                    { "QMIMER", "Mimer SQL" },
                                                    { "QOCI",   "Oracle (OCI)" },
                                                    { "QODBC",  "ODBC" },
                                                    { "QDB2", "IBM Db2" },
                                                   };

    return names.value(driver, driver);  // fallback : le code brut si inconnu
}

QStringList DatabaseConnection::supportedDrivers()
{
    return QSqlDatabase::drivers();
}