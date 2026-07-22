#include "databaseConnection.h"


DatabaseConnection::DatabaseConnection(const ConnectionInfo p_connectionInfo) : m_connectionInfo(p_connectionInfo)
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
        qDebug() << "Server Connection Failed:" << m_db->lastError().text();
        return;
    }
    qDebug() << "Successfully connected to MySQL!";
}

QString DatabaseConnection::displayName(const QString &driver)
{
    static const QHash<QString, QString> names = {
                                                    { "QPSQL",   "PostgreSQL" },
                                                    { "QSQLITE", "SQLite"     },
                                                    { "QMYSQL",  "MySQL / MariaDB" },
                                                    { "QIBASE", "InterBase / Firebird" },
                                                    { "QMIMER", "Mimer SQL" },
                                                    { "QOCI",   "Oracle (OCI)" },
                                                    { "QODBC",  "ODBC" },
                                                    { "QDB2", "IBM Db2" },
                                                   };

    return names.value(driver, driver);  // fallback : le code brut si inconnu
}

QStringList DatabaseConnection::supportedDrivers()
{
    return QSqlDatabase::drivers();
}