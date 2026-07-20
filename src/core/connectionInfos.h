#ifndef CONNECTIONINFOS_H
#define CONNECTIONINFOS_H

#include <QString>

class ConnectionInfo
{
public:
    ConnectionInfo(const QString& p_engine, const QString& p_host, const uint8_t p_port, const QString& p_database, const QString& p_username, const QString& p_password);

    const QString& getEngine()     const;
    const QString& getHost()       const;
          uint8_t  getPort()       const;
    const QString& getDatabase()   const;
    const QString& getUsername()   const;
    const QString& getPassword()   const;

private:
    QString m_engine;
    QString m_host;
    uint8_t m_port;
    QString m_database;
    QString m_username;
    QString m_password;
};

#endif // CONNECTIONINFOS_H
