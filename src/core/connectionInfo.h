#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QString>

class ConnectionInfo
{
public:
    ConnectionInfo(const QString &p_uuid, const QString &p_profileName, const QString& p_engine, const QString& p_host, const uint16_t p_port, const QString& p_database, const QString& p_username, const QString& p_password);

    const inline QString &getUuid()         const { return m_uuid; }
    const inline QString &getProfileName()  const { return m_profileName; }
    const inline QString &getEngine()       const { return m_engine; }
    const inline QString &getHost()         const { return m_host; }
          inline uint16_t getPort()         const { return m_port; }
    const inline QString &getDatabase()     const { return m_database; }
    const inline QString &getUsername()     const { return m_username; }
    const inline QString &getPassword()     const { return m_password; }

private:
    QString     m_uuid;
    QString     m_profileName;
    QString     m_engine;
    QString     m_host;
    uint16_t    m_port;
    QString     m_database;
    QString     m_username;
    QString     m_password;
};

#endif // CONNECTIONINFO_H
