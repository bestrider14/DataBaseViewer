#ifndef CONNECTIONPROFILESTORE_H
#define CONNECTIONPROFILESTORE_H

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QUuid>

#include <tuple>
#include <vector>

#include "core/connectionInfo.h"
#include "core/passwordManager.h"


class ConnectionProfileStore : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionProfileStore(QObject *p_parent = nullptr);
    void save(const ConnectionInfo &p_info);
    void load(const QString &p_uuid) const;
    std::vector<std::tuple<QString, QString>> getProfilesNameAndUuid();
    void erase(const QString &p_uuid);

private slots:
    void onSaved(const QString &p_uuid);
    void onLoaded(const QString &p_uuid, const QString &p_password);
    void onErased(const QString &p_uuid);
    void onSaveFailed(const QString &p_uuid);

signals:
    void errorMessage(const QString &p_title, const QString &p_message);
    void profileSaved(const QString &p_profileName, const QString &p_uuid);
    void profileLoaded(const ConnectionInfo &p_connectionInfo);
    void profileErased(const QString &p_uuid);

private:
    QSettings *m_profilesSaved = nullptr;
    PasswordManager *m_passwordManager = new PasswordManager(this);
};

#endif // CONNECTIONPROFILESTORE_H
