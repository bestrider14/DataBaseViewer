#include "mainWindow.h"
#include "dialogConnectionSettings.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    initUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::initDatabaseConnection(const ConnectionInfo &p_connectionInfo)
{
    m_databaseConnection = std::make_unique<DatabaseConnection>(p_connectionInfo);
    ui->tableData->setConnection(m_databaseConnection.get());

    ui->connectBtn->setEnabled(true);

    connect(m_databaseConnection.get(), &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
    connect(m_databaseConnection.get(), &DatabaseConnection::connected, this, &MainWindow::onDatabaseConnected);
    connect(m_databaseConnection.get(), &DatabaseConnection::disconnected, this, &MainWindow::onDatabaseDisconnected);
    connect(m_databaseConnection.get(), &DatabaseConnection::tablesListUpdated, ui->tableTreeExplorer, &TableExplorerWidget::setTables);
    connect(m_databaseConnection.get(), &DatabaseConnection::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);


}

void MainWindow::initUi()
{
    ui->setupUi(this);

    ui->connectBtn->setEnabled(false);

    statusBar()->addPermanentWidget(m_state,0);

    connect(ui->connectionSettingsBtn, &QPushButton::clicked, this, &MainWindow::onConnectionSettingsClicked);
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);

    connect(ui->addRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onAddRow);
    connect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onDeletingRow);

    connect(ui->tableTreeExplorer, &TableExplorerWidget::tableSelected, ui->tableData, &TableDataWidget::showTable);
    connect(ui->tableTreeExplorer, &TableExplorerWidget::tableSelected, this, &MainWindow::onTableSelected);

    connect(ui->tableData, &TableDataWidget::error, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);

    connect(ui->tableData, &TableDataWidget::addingRow, this, &MainWindow::onAddingRow);
    connect(ui->tableData, &TableDataWidget::canceled, this, &MainWindow::onCancel);

    connect(ui->tableData, &TableDataWidget::rowSelected, this, &MainWindow::onRowSelected);
    connect(ui->tableData, &TableDataWidget::noRowSelected, this, &MainWindow::onNoRowSelected);

}

void MainWindow::stateChanged(const QString &p_newState)
{
    m_state->setText(p_newState);
}

void MainWindow::receivedStatus(const QString &p_message, int p_timeout)
{
    statusBar()->showMessage(p_message, p_timeout);
}

void MainWindow::onCancel()
{
    ui->addRowBtn->setEnabled(true);
    ui->deleteRowBtn->setText("Delete row");
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
        m_databaseConnection->connect();
    else
        m_databaseConnection->disconnect();
}

void MainWindow::onTableSelected()
{
    ui->addRowBtn->setEnabled(true);
}

void MainWindow::onRowSelected()
{
    ui->deleteRowBtn->setEnabled(true);
}

void MainWindow::onNoRowSelected()
{
    ui->deleteRowBtn->setEnabled(false);
}

void MainWindow::onDatabaseDisconnected()
{
    m_isConnected = false;

    ui->tableData->clear();
    ui->tableTreeExplorer->clear();
    ui->connectBtn->setText("Connect");
    ui->connectionSettingsBtn->setEnabled(true);
    ui->addRowBtn->setEnabled(false);
    ui->deleteRowBtn->setEnabled(false);

    stateChanged("Disconnected");
}

void MainWindow::onDatabaseConnected()
{
    m_isConnected = true;
    ui->connectBtn->setText("Disconnect");
    ui->connectionSettingsBtn->setEnabled(false);
    stateChanged("Connected");
}

void MainWindow::onAddingRow()
{
    ui->addRowBtn->setEnabled(false);
    ui->deleteRowBtn->setText("Cancel");

    disconnect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onDeletingRow);
    connect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onCancel);
}

