#include <QString>
#include <QDebug>
#include <QLineEdit>

#include "dialogConnectionSettings.h"
#include "qpushbutton.h"
#include "ui_dialogConnectionSettings.h"
#include "core/databaseconnection.h"


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

    connect(ui->engineCombo, &QComboBox::currentIndexChanged, this, &DialogConnectionsSettings::OnEngineComboChanged);

    connect(ui->hostIpEdit,   &QLineEdit::textEdited, this, &DialogConnectionsSettings::OnEdit);
    connect(ui->hostPortEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::OnEdit);
    connect(ui->databaseEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::OnEdit);
    connect(ui->usernameEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::OnEdit);
    connect(ui->passwordEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::OnEdit);

}

DialogConnectionsSettings::~DialogConnectionsSettings()
{
    delete ui;
}

ConnectionInfo DialogConnectionsSettings::GetConnectionInfo()
{
    ConnectionInfo connectionInfo(ui->engineCombo->currentData().toString(),
                                  ui->hostIpEdit->text(),
                                  ui->hostPortEdit->text(),
                                  ui->databaseEdit->text(),
                                  ui->usernameEdit->text(),
                                  ui->passwordEdit->text());

    return connectionInfo;
}

void DialogConnectionsSettings::OnEngineComboChanged()
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

    OnEdit();
}

void DialogConnectionsSettings::OnEdit()
{
    if( ui->databaseEdit->text() == "" ||
        ui->usernameEdit->text() == "" ||
        ui->passwordEdit->text() == "")
    {
        setOkButtonEnable(false);
        return;
    }

    if(ui->engineCombo->currentData().toString() != "QSQLITE")
    {
        if( ui->hostIpEdit->text() == "" || ui->hostPortEdit->text() == "")
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
