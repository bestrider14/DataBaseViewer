#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include <QHash>
#include <QSqlError>
#include <QSqlQuery>

#include <optional>

#include "connectionInfo.h"

class DatabaseConnection
{
public:
    DatabaseConnection(const ConnectionInfo connectionInfo);

    static QString displayName(const QString &driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList supportedDrivers();

    void connect();

private:
    std::optional<ConnectionInfo> m_connectionInfo;
    std::optional<QSqlDatabase> m_db;

};

#endif // DATABASECONNECTION_H
