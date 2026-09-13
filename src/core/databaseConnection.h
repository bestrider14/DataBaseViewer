#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QHash>
#include <QSqlError>
#include <QUuid>
#include <QMessageBox>

#include "core/customTableModel.h"
#include "connectionInfo.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseConnection(const ConnectionInfo &p_connectionInfo, QObject *p_parent = nullptr);

    static QString      displayName(const QString &p_driver);   // "QPSQL" -> "PostgreSQL"
    static QStringList  supportedDrivers();

    void                connect();
    void                disconnect();

    CustomTableModel*                  getTableData(const QString &p_tableName) const;
    const  QMap<QString, QStringList>  getTablesList()  const;

    inline bool         isConnected()    const { return m_isConnected; }

public slots:
    void onExecuteRequested(const QString &p_query);

signals:
    void errorMessage(const QString &p_title, const QString &p_message);
    void disconnected();
    void failedConnection();
    void successfullConnection(const QString &p_profileName, const QString &p_uuid);
    void selectedQueryExecuted(QSqlQueryModel *p_model);
    void numRowsAffected(int p_numOfRowsAffected);

private:
    ConnectionInfo  m_connectionInfo;
    QSqlDatabase    m_db;
    bool            m_isConnected = false;
};

#endif // DATABASECONNECTION_H
