#include "databaseConnection.h"


DatabaseConnection::DatabaseConnection(const ConnectionInfo p_connectionInfo, QObject *p_parent) : QObject(p_parent), m_connectionInfo(p_connectionInfo)
{
}

void DatabaseConnection::connect()
{
    m_db = QSqlDatabase::addDatabase(m_connectionInfo->getEngine());

    if(m_connectionInfo->getEngine() != "QSQLITE")
    {
        m_db->setHostName(m_connectionInfo->getHost());
        m_db->setPort(m_connectionInfo->getPort());
    }

    m_db->setDatabaseName(m_connectionInfo->getDatabase());
    m_db->setUserName(m_connectionInfo->getUsername());
    m_db->setPassword(m_connectionInfo->getPassword());

    if (!m_db->open()) {
        statusMessage("Server Connection Failed: " + m_db->lastError().text());
        disconnected();
        return;
    }
    statusMessage("Server Connection Succeful");
    connected();
    updateTablesList();
}

void DatabaseConnection::disconnect()
{
    m_db->close();
    disconnected();
}

void DatabaseConnection::updateTablesList()
{
    tablesListUpdated(m_db->tables());
}

void DatabaseConnection::onItemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << item->text(0);
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