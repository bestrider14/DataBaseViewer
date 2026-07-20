#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>

#include "dialogConnectionSettings.h"
#include "ui_dialogConnectionSettings.h"
#include "core/databaseConnection.h"


DialogConnectionsSettings::DialogConnectionsSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConnectionsSettings)
{
    ui->setupUi(this);

    setOkButtonEnable(false);

    for (const QString &driver : DatabaseConnection::supportedDrivers())
    {
        ui->engineCombo->addItem(DatabaseConnection::displayName(driver),driver);
    }

    onEngineComboChanged();

    connect(ui->engineCombo, &QComboBox::currentIndexChanged, this, &DialogConnectionsSettings::onEngineComboChanged);

    connect(ui->hostIpEdit,   &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->hostPortEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->databaseEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->usernameEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->passwordEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);

}

DialogConnectionsSettings::~DialogConnectionsSettings()
{
    delete ui;
}

ConnectionInfo DialogConnectionsSettings::getConnectionInfo() const
{
    ConnectionInfo connectionInfo(ui->engineCombo->currentData().toString(),
                                  ui->hostIpEdit->text(),
                                  ui->hostPortEdit->text().toInt(),
                                  ui->databaseEdit->text(),
                                  ui->usernameEdit->text(),
                                  ui->passwordEdit->text());


    return connectionInfo;
}

void DialogConnectionsSettings::onEngineComboChanged()
{
    QString currentData = ui->engineCombo->currentData().toString();

    if (currentData == "QSQLITE")
    {
        ui->hostIpEdit->setDisabled(true);
        ui->hostPortEdit->setDisabled(true);
    }
    else
    {
        ui->hostIpEdit->setDisabled(false);
        ui->hostPortEdit->setDisabled(false);
    }

    onEdit();
}

void DialogConnectionsSettings::onEdit()
{
    if( ui->databaseEdit->text().isEmpty() ||
        ui->usernameEdit->text().isEmpty() ||
        ui->passwordEdit->text().isEmpty())
    {
        setOkButtonEnable(false);
        return;
    }

    if(ui->engineCombo->currentData().toString() != "QSQLITE")
    {
        if( ui->hostIpEdit->text().isEmpty() || ui->hostPortEdit->text().isEmpty())
        {
            setOkButtonEnable(false);
            return;
        }
    }

    setOkButtonEnable(true);
}

void DialogConnectionsSettings::setOkButtonEnable(bool b)
{
    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(b);
}
