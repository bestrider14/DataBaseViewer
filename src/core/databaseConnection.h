#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>

#include "connectionInfos.h"

class DatabaseConnection
{
public:
    DatabaseConnection(ConnectionInfo connectionInfo);

    static QString displayName(const QString &driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList supportedDrivers();

private:
    QString m_engine;
    QString m_host;
    uint8_t m_port;
    QString m_database;
    QString m_username;
    QString m_password;
};

#endif // DATABASECONNECTION_H
