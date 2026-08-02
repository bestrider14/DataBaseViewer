#include "databaseConnection.h"

DatabaseConnection::DatabaseConnection(const ConnectionInfo &p_connectionInfo, QObject *p_parent) : QObject(p_parent), m_connectionInfo(p_connectionInfo)
{}

void DatabaseConnection::connect()
{
    m_databaseConnectionName = generateUuid();
    m_db = QSqlDatabase::addDatabase(m_connectionInfo.getEngine(), m_databaseConnectionName);

    if(m_connectionInfo.getEngine() != "QSQLITE")
    {
        m_db.setHostName(m_connectionInfo.getHost());
        m_db.setPort(m_connectionInfo.getPort());
    }

    m_db.setDatabaseName(m_connectionInfo.getDatabase());
    m_db.setUserName(m_connectionInfo.getUsername());
    m_db.setPassword(m_connectionInfo.getPassword());

    if (!m_db.open())
    {
        emit errorMessage("Connection failed", m_db.lastError().text());
        disconnect();
        return;
    }

    emit statusMessage("Server Connection Succeful");
    emit connected();

    updateTablesList();
}

void DatabaseConnection::disconnect()
{
    m_db.close();
    m_db = QSqlDatabase();
    m_db.removeDatabase(m_databaseConnectionName);
    emit disconnected();
}

CustomTableModel* DatabaseConnection::getTableData(const QString &p_tableName) const
{
    CustomTableModel *model = new CustomTableModel(nullptr, m_db);
    model->setTable(p_tableName);
    model->select();

    return model;
}

void DatabaseConnection::updateTablesList()
{
    emit tablesListUpdated(m_db.tables());
}


QString DatabaseConnection::displayName(const QString &p_driver)
{
    static const QHash<QString, QString> names = {
                                                    { "QPSQL",      "PostgreSQL" },
                                                    { "QSQLITE",    "SQLite"     },
                                                    { "QMYSQL",     "MySQL / MariaDB" },
                                                    { "QIBASE",     "InterBase / Firebird" },
                                                    { "QMIMER",     "Mimer SQL" },
                                                    { "QOCI",       "Oracle (OCI)" },
                                                    { "QODBC",      "ODBC" },
                                                    { "QDB2",       "IBM Db2" },
                                                   };

    return names.value(p_driver, p_driver);  // fallback : le code brut si inconnu
}

QStringList DatabaseConnection::supportedDrivers()
{
    return QSqlDatabase::drivers();
}

QString DatabaseConnection::generateUuid() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}