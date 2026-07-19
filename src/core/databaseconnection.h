#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>

class DatabaseConnection
{
public:
    DatabaseConnection();

    static QString displayName(const QString &driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList supportedDrivers();
};

#endif // DATABASECONNECTION_H
