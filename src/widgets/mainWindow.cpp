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

void MainWindow::initUi()
{
    ui->setupUi(this);

    m_messageBox->hide();

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);

    ui->connectBtn->setEnabled(false);

    statusBar()->addPermanentWidget(m_state,0);

    auto profilesInfo = m_profiles->profileNames();

    if(!profilesInfo.empty())
    {
        for(const auto &tuple : profilesInfo)
            ui->profilesListComboBox->addItem(std::get<0>(tuple), std::get<1>(tuple));

        ui->connectBtn->setEnabled(true);
        ui->deleteProfileBtn->setEnabled(true);
        ui->profilesListComboBox->setEnabled(true);
        onComboBoxChanged(ui->profilesListComboBox->currentIndex());
    }
    else
    {
        ui->profilesListComboBox->setPlaceholderText("Empty");
        ui->profilesListComboBox->setEnabled(false);
    }

    connect(ui->addProfileBtn, &QPushButton::clicked, this, &MainWindow::onAddProfileClicked);
    connect(ui->deleteProfileBtn, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->profilesListComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onComboBoxChanged);

    connect(ui->addRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onAddRow);
    connect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onDeletingRow);

    connect(ui->tableTreeExplorer, &TableExplorerWidget::tableSelected, ui->tableData, &TableDataWidget::showTable);
    connect(ui->tableTreeExplorer, &TableExplorerWidget::tableSelected, this, &MainWindow::onTableSelected);

    connect(ui->tableData, &TableDataWidget::error, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
    connect(ui->tableData, &TableDataWidget::addingRow, this, &MainWindow::onAddingRow);
    connect(ui->tableData, &TableDataWidget::canceled, this, &MainWindow::onCancel);
    connect(ui->tableData, &TableDataWidget::columnSelected, ui->searchBarWidget, &SearchBarWidget::onColumnSelected);
    connect(ui->tableData, &TableDataWidget::rowSelected, this, &MainWindow::onRowSelected);
    connect(ui->tableData, &TableDataWidget::noRowSelected, this, &MainWindow::onNoRowSelected);

    connect(ui->searchBarWidget, &SearchBarWidget::lineEditIsEmpty, ui->tableData, &TableDataWidget::resetFilter);
    connect(ui->searchBarWidget, &SearchBarWidget::searchRequested, ui->tableData, &TableDataWidget::onSearchRequested);

    connect(m_profiles, &ConnectionProfileStore::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
    connect(m_profiles, &ConnectionProfileStore::profileLoaded, this, &MainWindow::onProfileLoaded);
    connect(m_profiles, &ConnectionProfileStore::profileErased, this, &MainWindow::onProfileErased);
    connect(m_profiles, &ConnectionProfileStore::profileSaved, this, &MainWindow::onProfileSaved);
}

void MainWindow::stateChanged(const QString &p_newState)
{
    m_state->setText(p_newState);
}

void MainWindow::setConnection(const QString &p_uuid)
{
    auto databaseConnection = m_databaseConnections[p_uuid].get();
    ui->tableData->setConnection(databaseConnection);

    if(databaseConnection->isConnected())
    {
        onDatabaseConnected();
        ui->tableTreeExplorer->setTables(databaseConnection->getTablesList());
        ui->tableData->showTable(m_tableSelectedList[p_uuid]);
    }
    else
        onDatabaseDisconnected();
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

void MainWindow::onDeleteProfileClicked()
{

    /// voir pour le dernier profile pop erreur

    QString uuid = ui->profilesListComboBox->currentData().toString();
    auto iterator = m_databaseConnections.find(uuid);

    if(iterator == m_databaseConnections.end())
    {
        m_messageBox->onErrorMessage("Error", "Error while deleting profile.");
            return;
    }

    if(iterator->second->isConnected())
        onConnectClicked();

    m_profiles->erase(ui->profilesListComboBox->currentData().toString());
}

void MainWindow::onComboBoxChanged(int p_index)
{

    if(p_index == -1)
        return;

    QString uuid = ui->profilesListComboBox->itemData(p_index).toString();

    auto iterator = m_databaseConnections.find(uuid);

    if(iterator == m_databaseConnections.end())
    {
        m_profiles->load(uuid);
        ui->connectBtn->setText("Loading...");
        ui->connectBtn->setEnabled(false);
        ui->profilesListComboBox->setEnabled(false);
        ui->deleteProfileBtn->setEnabled(false);
        return;
    }
    else
    {
        setConnection(uuid);
        return;
    }
}

void MainWindow::onProfileSaved(const QString &p_profileName, const QString &p_uuid)
{
    ui->profilesListComboBox->addItem(p_profileName, p_uuid);
    ui->profilesListComboBox->setCurrentIndex(ui->profilesListComboBox->count()-1);
    ui->profilesListComboBox->setEnabled(true);
    ui->deleteProfileBtn->setEnabled(true);
}

void MainWindow::onProfileLoaded(const QString &p_uuid, const ConnectionInfo &p_connectionInfo)
{
    if(p_uuid != ui->profilesListComboBox->currentData().toString())
    {
        m_messageBox->onErrorMessage("Error", "An error occur while loading the profile.");
        return;
    }

    m_databaseConnections.insert({p_uuid , std::make_unique<DatabaseConnection>(p_connectionInfo)});
    setConnection(p_uuid);
    ui->deleteProfileBtn->setEnabled(true);
    ui->profilesListComboBox->setEnabled(true);

    auto databaseConnection = m_databaseConnections[p_uuid].get();
    connect(databaseConnection, &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
    connect(databaseConnection, &DatabaseConnection::connected, this, &MainWindow::onDatabaseConnected);
    connect(databaseConnection, &DatabaseConnection::disconnected, this, &MainWindow::onDatabaseDisconnected);
    connect(databaseConnection, &DatabaseConnection::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
}

void MainWindow::onProfileErased(const QString &p_uuid)
{
    if(p_uuid != ui->profilesListComboBox->currentData().toString())
    {
        m_messageBox->onErrorMessage("Error", "An error occur while erasing the profile.");
        return;
    }

    ui->profilesListComboBox->removeItem(ui->profilesListComboBox->currentIndex());

    m_databaseConnections.erase(p_uuid);
    m_tableSelectedList.erase(p_uuid);

    if(m_profiles->profileNames().empty())
    {
        ui->profilesListComboBox->setPlaceholderText("Empty");
        ui->profilesListComboBox->setEnabled(false);
        ui->connectBtn->setEnabled(false);
        ui->deleteProfileBtn->setEnabled(false);
    }
}

void MainWindow::onAddProfileClicked()
{
    DialogConnectionsSettings dialog(this);

    if (dialog.exec() == QDialog::Accepted)
        m_profiles->save(dialog.getConnectionInfo());
}

void MainWindow::onConnectClicked()
{

    auto databaseConnection = m_databaseConnections[ui->profilesListComboBox->currentData().toString()].get();

    if(!databaseConnection->isConnected())
    {
        databaseConnection->connect();
        ui->tableTreeExplorer->setTables(databaseConnection->getTablesList());
    }
    else
    {
        databaseConnection->disconnect();
        disconnect(databaseConnection, &DatabaseConnection::statusMessage, this, &MainWindow::receivedStatus);
        disconnect(databaseConnection, &DatabaseConnection::connected, this, &MainWindow::onDatabaseConnected);
        disconnect(databaseConnection, &DatabaseConnection::disconnected, this, &MainWindow::onDatabaseDisconnected);
        disconnect(databaseConnection, &DatabaseConnection::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
    }
}

void MainWindow::onTableSelected(const QString &p_tableName)
{
    ui->addRowBtn->setEnabled(true);

    QString selectedProfile = ui->profilesListComboBox->currentData().toString();

    m_tableSelectedList[selectedProfile] = p_tableName;
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
    ui->tableData->clear();
    ui->tableTreeExplorer->clear();
    ui->connectBtn->setText("Connect");
    ui->addRowBtn->setEnabled(false);
    ui->deleteRowBtn->setEnabled(false);
    ui->connectBtn->setEnabled(true);

    stateChanged("Disconnected");
}

void MainWindow::onDatabaseConnected()
{
    ui->connectBtn->setText("Disconnect");
    stateChanged("Connected");
}

void MainWindow::onAddingRow()
{
    ui->addRowBtn->setEnabled(false);
    ui->deleteRowBtn->setText("Cancel");

    disconnect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onDeletingRow);
    connect(ui->deleteRowBtn, &QPushButton::clicked, ui->tableData, &TableDataWidget::onCancel);
}

