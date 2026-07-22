#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include <QHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QErrorMessage>

#include <optional>

#include "connectionInfo.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseConnection(const ConnectionInfo p_connectionInfo, QObject *p_parent = nullptr);

    static QString displayName(const QString &p_driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList supportedDrivers();

    void connect();

signals:
    void statusMessage(const QString &p_message, int p_timeout = 5000);
    void connected(const QString &p_message = "Connected");
    void disconnected(const QString &p_message = "Disconnected");

private:
    std::optional<ConnectionInfo> m_connectionInfo;
    std::optional<QSqlDatabase> m_db;

};

#endif // DATABASECONNECTION_H
