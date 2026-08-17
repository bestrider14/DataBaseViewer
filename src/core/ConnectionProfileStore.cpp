#include "connectionProfileStore.h"

ConnectionProfileStore::ConnectionProfileStore(QObject *p_parent) : QObject(p_parent)
{
    connect(m_passwordManager, &PasswordManager::errorMessage, this, [this](const QString &p_title, const QString &p_message){ emit errorMessage(p_title,p_message); });
    connect(m_passwordManager, &PasswordManager::loaded, this, &ConnectionProfileStore::onLoaded);
    connect(m_passwordManager, &PasswordManager::erased, this, &ConnectionProfileStore::onErased);
    connect(m_passwordManager, &PasswordManager::saved, this, &ConnectionProfileStore::onSaved);
}

void ConnectionProfileStore::save(const ConnectionInfo &p_info)
{
    if(!m_profilesSaved.isWritable())
    {
        emit errorMessage("Error", "Error while saving your connection");
        return;
    }

    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    m_profilesSaved.beginGroup(uuid);
    m_profilesSaved.setValue("profileName", p_info.getProfileName());
    m_profilesSaved.setValue("database", p_info.getDatabase());
    m_profilesSaved.setValue("engine", p_info.getEngine());
    m_profilesSaved.setValue("host", p_info.getHost());
    m_profilesSaved.setValue("port", p_info.getPort());
    m_profilesSaved.setValue("username", p_info.getUsername());
    m_profilesSaved.endGroup();

    m_passwordManager->save(uuid, p_info.getPassword());
}

void ConnectionProfileStore::load(const QString &p_uuid) const
{
    m_passwordManager->load(p_uuid);
}

std::vector<std::tuple<QString, QString>> ConnectionProfileStore::profileNames()
{
    std::vector<std::tuple<QString, QString>> profileNames;

    for (const QString &uuid : m_profilesSaved.childGroups())
    {
        m_profilesSaved.beginGroup(uuid);
        QString profileName = m_profilesSaved.value("profileName").toString();
        m_profilesSaved.endGroup();

        profileNames.emplace_back(profileName, uuid);
    }

    return profileNames;
}

void ConnectionProfileStore::erase(const QString &p_uuid)
{
    m_passwordManager->erase(p_uuid);
}

void ConnectionProfileStore::onSaved(const QString &p_uuid)
{
    m_profilesSaved.beginGroup(p_uuid);
    emit profileSaved(m_profilesSaved.value("profileName").toString(), p_uuid);
    m_profilesSaved.endGroup();
}

void ConnectionProfileStore::onLoaded(const QString &p_uuid, const QString &p_password)
{
    m_profilesSaved.beginGroup(p_uuid);

    emit profileLoaded (p_uuid,
        ConnectionInfo(
        m_profilesSaved.value("profileName").toString(),
        m_profilesSaved.value("engine").toString(),
        m_profilesSaved.value("host").toString(),
        m_profilesSaved.value("port").toUInt(),
        m_profilesSaved.value("database").toString(),
        m_profilesSaved.value("username").toString(),
        p_password));

    m_profilesSaved.endGroup();
}

void ConnectionProfileStore::onErased(const QString &p_uuid)
{
    m_profilesSaved.remove(p_uuid);
    emit profileErased(p_uuid);
}