#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QString>

class ConnectionInfo
{
public:
    ConnectionInfo(QString p_engine, QString p_hostIp, QString p_port, QString p_database, QString p_username, QString p_password);

    QString GetEngine()     const;
    QString GetHost()       const;
    QString GetPort()       const;
    QString GetDatabase()   const;
    QString GetUsername()   const;
    QString GetPassword()   const;

private:
    QString m_engine;
    QString m_hostIp;
    QString m_port;
    QString m_database;
    QString m_username;
    QString m_password;
};

#endif // CONNECTIONINFO_H
