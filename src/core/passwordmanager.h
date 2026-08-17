#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QObject>
#include <QString>

class PasswordManager : public QObject
{
    Q_OBJECT
public:
    explicit PasswordManager(QObject *p_parent = nullptr);

public:
    void save(const QString &p_uuid, const QString &p_password);
    void load(const QString &p_key);
    void erase(const QString &p_key);

signals:
    void saved(const QString &p_uuid);
    void loaded(const QString &p_key, const QString &p_password);
    void erased(const QString &p_key);
    void errorMessage(const QString &p_title, const QString &p_message);

private:
    inline static const QString KEY_PREFIX = "password/";

};

#endif // PASSWORDMANAGER_H
