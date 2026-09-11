#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include "databaseConnection.h"

DatabaseConnection::DatabaseConnection(const ConnectionInfo &p_connectionInfo, QObject *p_parent) : QObject(p_parent), m_connectionInfo(p_connectionInfo)
{}

void DatabaseConnection::connect()
{
    m_db = QSqlDatabase::addDatabase(m_connectionInfo.getEngine(), m_connectionInfo.getUuid());

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
        m_isConnected = false;
        emit errorMessage("Connection failed", m_db.lastError().text());
        emit failedConnection();
        return;
    }

    m_isConnected = true;
    emit successfullConnection(m_connectionInfo.getProfileName(), m_connectionInfo.getUuid());
}

void DatabaseConnection::disconnect()
{
    m_db.close();
    m_db = QSqlDatabase();
    m_db.removeDatabase(m_connectionInfo.getUuid());
    m_isConnected = false;
    emit disconnected();
}

CustomTableModel* DatabaseConnection::getTableData(const QString &p_tableName) const
{
    CustomTableModel *model = new CustomTableModel(nullptr, m_db);
    model->setTable(p_tableName);
    model->select();

    return model;
}

const QMap<QString, QStringList> DatabaseConnection::getTablesList() const
{
    QMap<QString, QStringList> map;

    auto engine = m_connectionInfo.getEngine();

    if(engine == "QPSQL" || engine == "QMYSQL")
    {
        QSqlQuery query("SELECT table_schema, table_name, table_type FROM information_schema.tables WHERE table_type = 'BASE TABLE'", m_db);

        while(query.next())
        {
            QString schema = query.value(0).toString();
            QString table = query.value(1).toString();

            map[schema].append(table);
        }
    }
    else
    {
        map.insert("NO_SCHEMA", m_db.tables());
    }
    return map;
}

void DatabaseConnection::onExecuteRequested(const QString &p_query)
{
    QSqlQuery query(m_db);

    if (!query.exec(p_query))
    {
        emit errorMessage("Error SQL", query.lastError().text());
        return;
    }

    if(query.isSelect())
    {
        auto *model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        emit selectedQueryExecuted(model);
    }
    else
    {
        emit numRowsAffected(query.numRowsAffected());
    }
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