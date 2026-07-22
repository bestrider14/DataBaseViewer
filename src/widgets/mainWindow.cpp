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

    statusBar()->addPermanentWidget(state,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::initDatabaseConnection(const ConnectionInfo& p_connectionInfo)
{
     m_databaseConnection.emplace(p_connectionInfo);

     ui->connectBtn->setEnabled(m_databaseConnection.has_value());

     connect(&m_databaseConnection.value(), &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
     connect(&m_databaseConnection.value(), &DatabaseConnection::connected, this, &MainWindow::stateChanged);
}

void MainWindow::stateChanged(const QString& p_newState)
{
    state->setText(p_newState);
}

void MainWindow::receivedStatus(const QString& p_message, int p_timeout)
{
    statusBar()->showMessage(p_message, p_timeout);
}

void MainWindow::onConnectionSettingsClicked()
{
    DialogConnectionsSettings dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        initDatabaseConnection(dialog.getConnectionInfo());
    }
}

void MainWindow::onConnectClicked()
{
    m_databaseConnection->connect();
}

