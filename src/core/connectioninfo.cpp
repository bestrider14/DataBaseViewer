#include "connectioninfo.h"

ConnectionInfo::ConnectionInfo(const QString& p_engine, const QString& p_hostIp, const QString& p_port, const QString& p_database, const QString& p_username, const QString& p_password) :
    m_engine(p_engine), m_hostIp(p_hostIp), m_port(p_port), m_database(p_database), m_username(p_username), m_password(p_password)
{

}

const QString& ConnectionInfo::getEngine() const
{
    return m_engine;
}

const QString& ConnectionInfo::getHost() const
{
    return m_hostIp;
}

const QString& ConnectionInfo::getPort() const
{
    return m_port;
}

const QString& ConnectionInfo::getDatabase() const
{
    return m_database;
}

const QString& ConnectionInfo::getUsername() const
{
    return m_username;
}

const QString& ConnectionInfo::getPassword() const
{
    return m_password;
}
