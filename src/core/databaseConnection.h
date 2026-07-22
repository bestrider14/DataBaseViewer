#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include <QHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QErrorMessage>
#include <QTreeWidgetItem>

#include <optional>

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

private:
    void updateTablesList();

public slots:
    void onItemClicked(QTreeWidgetItem *item, int column);

signals:
    void statusMessage(const QString &p_message, int p_timeout = 5000) const;
    void connected(const QString &p_message = "Connected") const;
    void disconnected(const QString &p_message = "Disconnected") const;
    void tablesListUpdated(const QStringList &p_tableList);

private:
    ConnectionInfo m_connectionInfo;
    std::unique_ptr<QSqlDatabase> m_db;

};

#endif // DATABASECONNECTION_H
