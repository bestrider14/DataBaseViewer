#include "mainWindow.h"
#include "dialogConnectionSettings.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->connectBtn->setEnabled(false);

    connect(ui->connectionSettingsBtn, &QPushButton::clicked, this, &MainWindow::onConnectionSettingsClicked);
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectionSettingsClicked()
{
    DialogConnectionsSettings dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_databaseConnection = DatabaseConnection(dialog.getConnectionInfo());
        ui->connectBtn->setEnabled(m_databaseConnection.has_value());
    }
}

void MainWindow::onConnectClicked()
{
    m_databaseConnection->connect();
}

