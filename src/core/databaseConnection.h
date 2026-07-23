#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include <QHash>
#include <QSqlError>
#include <QSqlTableModel>
#include <QErrorMessage>
#include <QUuid>
#include <QMessageBox>

#include "connectionInfo.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseConnection(const ConnectionInfo &p_connectionInfo, QObject *p_parent = nullptr);

    static QString displayName(const QString &p_driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList supportedDrivers();

    void connect();
    void disconnect();
    QSqlTableModel* getTableData(const QString &p_tableName) const;

private:
    void updateTablesList();
    QString generateUuid() const;

signals:
    void statusMessage(const QString &p_message, int p_timeout = 5000) const;
    void connected(const QString &p_message = "Connected") const;
    void disconnected(const QString &p_message = "Disconnected") const;
    void tablesListUpdated(const QStringList &p_tableList);

private:
    ConnectionInfo m_connectionInfo;
    QSqlDatabase m_db;

    QString m_databaseConnectionName;

};

#endif // DATABASECONNECTION_H
