#include "databaseConnection.h"
#include <QHash>
#include <QSqlDatabase>

DatabaseConnection::DatabaseConnection(ConnectionInfo connectionInfo) :
    m_engine(connectionInfo.getEngine()),
    m_host(connectionInfo.getHost()),
    m_port(connectionInfo.getPort()),
    m_database(connectionInfo.getDatabase()),
    m_username(connectionInfo.getUsername()),
    m_password(connectionInfo.getPassword())
{

}

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