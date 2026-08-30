#include "connectionInfo.h"

ConnectionInfo::ConnectionInfo(const QString &p_uuid, const QString &p_profileName, const QString &p_engine, const QString &p_host, const uint16_t p_port, const QString &p_database, const QString &p_username, const QString &p_password) :
    m_uuid(p_uuid), m_profileName(p_profileName), m_engine(p_engine), m_host(p_host), m_port(p_port), m_database(p_database), m_username(p_username), m_password(p_password)
{}
