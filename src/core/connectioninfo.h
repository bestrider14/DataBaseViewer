#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QString>

class ConnectionInfo
{
public:
    ConnectionInfo(const QString& p_engine, const QString& p_hostIp, const QString& p_port, const QString& p_database, const QString& p_username, const QString& p_password);

    const QString& getEngine()     const;
    const QString& getHost()       const;
    const QString& getPort()       const;
    const QString& getDatabase()   const;
    const QString& getUsername()   const;
    const QString& getPassword()   const;

private:
    QString m_engine;
    QString m_hostIp;
    QString m_port;
    QString m_database;
    QString m_username;
    QString m_password;
};

#endif // CONNECTIONINFO_H
