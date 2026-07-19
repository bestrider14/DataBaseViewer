#include "connectioninfo.h"

ConnectionInfo::ConnectionInfo(QString p_engine, QString p_hostIp, QString p_port, QString p_database, QString p_username, QString p_password) :
    m_engine(p_engine), m_hostIp(p_hostIp), m_port(p_port), m_database(p_database), m_username(p_username), m_password(p_password)
{

}

QString ConnectionInfo::GetEngine() const
{
    return m_engine;
}

QString ConnectionInfo::GetHost() const
{
    return m_hostIp;
}

QString ConnectionInfo::GetPort() const
{
    return m_port;
}

QString ConnectionInfo::GetDatabase() const
{
    return m_database;
}

QString ConnectionInfo::GetUsername() const
{
    return m_username;
}

QString ConnectionInfo::GetPassword() const
{
    return m_password;
}
