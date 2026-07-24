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


    statusBar()->addPermanentWidget(m_state,0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_model;
}

 void MainWindow::initDatabaseConnection(const ConnectionInfo &p_connectionInfo)
{
    m_databaseConnection = std::make_unique<DatabaseConnection>(p_connectionInfo);

    ui->connectBtn->setEnabled(true);

    connect(m_databaseConnection.get(), &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
    connect(m_databaseConnection.get(), &DatabaseConnection::connected, this, &MainWindow::onDatabaseConnected);
    connect(m_databaseConnection.get(), &DatabaseConnection::disconnected, this, &MainWindow::onDatabaseDisconnected);
    connect(m_databaseConnection.get(), &DatabaseConnection::tablesListUpdated, this, &MainWindow::updateTablesTree);

    connect(ui->tablesTree, &QTreeWidget::itemClicked, this, &MainWindow::onTreeItemClicked);
    connect(this, &MainWindow::tableSelected, this, &MainWindow::onTableSelected);

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

void MainWindow::onTableSelected(const QString &p_tableName)
{
    delete m_model;
    m_model = m_databaseConnection->getTableData(p_tableName);
    ui->tableContentView->setModel(m_model);
    ui->tableContentView->show();
}

void MainWindow::stateChanged(const QString &p_newState)
{
    m_state->setText(p_newState);
}

void MainWindow::receivedStatus(const QString &p_message, int p_timeout)
{
    statusBar()->showMessage(p_message, p_timeout);
}

void MainWindow::onTreeItemClicked(const QTreeWidgetItem *p_item)
{
    emit tableSelected(p_item->text(0));
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
    if(!m_isConnected)
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
    m_isConnected = false;
    ui->connectBtn->setText("Connect");
    ui->connectionSettingsBtn->setEnabled(true);
    stateChanged("Disconnected");
}

void MainWindow::onDatabaseConnected()
{
    m_isConnected = true;
    ui->connectBtn->setText("Disconnect");
    ui->connectionSettingsBtn->setEnabled(false);
    stateChanged("Connected");
}

