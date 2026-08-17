#include "dialogConnectionSettings.h"
#include "core/databaseConnection.h"


DialogConnectionsSettings::DialogConnectionsSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConnectionsSettings)
{
    ui->setupUi(this);
    m_saveButton = ui->buttonBox->button(QDialogButtonBox::Save);
    m_saveButton->setEnabled(false);

    for (const QString &driver : DatabaseConnection::supportedDrivers())
        ui->engineCombo->addItem(DatabaseConnection::displayName(driver),driver);

    onEngineComboChanged();

    connect(ui->engineCombo, &QComboBox::currentIndexChanged, this, &DialogConnectionsSettings::onEngineComboChanged);

    connect(ui->profileNameEdit, &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->hostIpEdit,      &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->hostPortEdit,    &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->databaseEdit,    &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->usernameEdit,    &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);
    connect(ui->passwordEdit,    &QLineEdit::textEdited, this, &DialogConnectionsSettings::onEdit);

}

DialogConnectionsSettings::~DialogConnectionsSettings()
{
    delete ui;
}

const ConnectionInfo DialogConnectionsSettings::getConnectionInfo() const
{
    ConnectionInfo connectionInfo(
        ui->profileNameEdit->text(),
        ui->engineCombo->currentData().toString(),
        ui->hostIpEdit->text(),
        ui->hostPortEdit->text().toUInt(),
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
        ui->passwordEdit->text().isEmpty() ||
        ui->profileNameEdit->text().isEmpty())
    {
        m_saveButton->setEnabled(false);
        return;
    }

    if(ui->engineCombo->currentData().toString() != "QSQLITE")
    {
        if( ui->hostIpEdit->text().isEmpty() || ui->hostPortEdit->text().isEmpty())
        {
            m_saveButton->setEnabled(false);
            return;
        }
    }

    m_saveButton->setEnabled(true);
}