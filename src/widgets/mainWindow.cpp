#include "mainWindow.h"
#include "dialogConnectionSettings.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->connectBtn->setEnabled(false);
    ui->tablesTree->setHeaderLabel("Database Tables");

    connect(ui->connectionSettingsBtn, &QPushButton::clicked, this, &MainWindow::onConnectionSettingsClicked);
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);


    statusBar()->addPermanentWidget(state,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::initDatabaseConnection(const ConnectionInfo &p_connectionInfo)
{
    m_databaseConnection.emplace(p_connectionInfo);

    ui->connectBtn->setEnabled(m_databaseConnection.has_value());

    connect(&m_databaseConnection.value(), &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
    connect(&m_databaseConnection.value(), &DatabaseConnection::connected, this, &MainWindow::onDatabaseConnected);
    connect(&m_databaseConnection.value(), &DatabaseConnection::disconnected, this, &MainWindow::onDatabaseDisconnected);
    connect(&m_databaseConnection.value(), &DatabaseConnection::tablesListUpdated, this, &MainWindow::updateTablesTree);

    connect(ui->tablesTree, &QTreeWidget::itemClicked, &m_databaseConnection.value(), &DatabaseConnection::onItemClicked);

}

void MainWindow::updateTablesTree(const QStringList &p_tableList)
{
    for (const QString &table : p_tableList)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, table);
        ui->tablesTree->addTopLevelItem(item);
    }
}

void MainWindow::stateChanged(const QString &p_newState)
{
    state->setText(p_newState);
}

void MainWindow::receivedStatus(const QString &p_message, int p_timeout)
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
    if(!isConnected)
    {
        m_databaseConnection->connect();
    }
    else
    {
        m_databaseConnection->disconnect();
    }
}

void MainWindow::onDatabaseDisconnected()
{
    ui->tablesTree->clear();
    isConnected = false;
    ui->connectBtn->setText("Connect");
}

void MainWindow::onDatabaseConnected()
{
    isConnected = true;
    ui->connectBtn->setText("Disconnect");
}

