#include "passwordManager.h"
#include "vendors/qtkeychain/qtkeychain/keychain.h"

PasswordManager::PasswordManager(QObject *parent)
    : QObject{parent}
{}

void PasswordManager::save(const QString &p_uuid, const QString &p_password)
{
    auto *job = new QKeychain::WritePasswordJob(p_uuid);

    job->setKey(p_uuid);
    job->setTextData(p_password);

    connect(job, &QKeychain::Job::finished,
            this,
            [this, p_uuid](QKeychain::Job *job)
            {
                if (job->error())
                {
                    emit errorMessage("Error", "Unable to save password: " + job->errorString());
                    job->deleteLater();
                    return;
                }
                emit saved(p_uuid);
                job->deleteLater();
            });

    job->start();
}

void PasswordManager::load(const QString &p_uuid)
{
    auto *job = new QKeychain::ReadPasswordJob(p_uuid);

    job->setKey(p_uuid);

    connect(job, &QKeychain::ReadPasswordJob::finished,
            this,
            [this, p_uuid](QKeychain::Job *job)
            {
                auto *readJob = qobject_cast<QKeychain::ReadPasswordJob *>(job);

                if (job->error())
                {
                    emit errorMessage("Erreur:", "Unable to load password: " + job->errorString());
                    job->deleteLater();
                    return;
                }

                emit loaded(p_uuid ,readJob->textData());
                job->deleteLater();
            });

    job->start();
}

void PasswordManager::erase(const QString &p_uuid)
{
    auto *job = new QKeychain::DeletePasswordJob(p_uuid);

    job->setKey(p_uuid);

    connect(job, &QKeychain::DeletePasswordJob::finished,
            this,
            [this, p_uuid](QKeychain::Job *job)
            {
                if (job->error())
                {
                    emit errorMessage("Erreur:", "Unable to delete password: " + job->errorString());
                    job->deleteLater();
                    return;
                }

                emit erased(p_uuid);
                job->deleteLater();
            });

    job->start();
}
