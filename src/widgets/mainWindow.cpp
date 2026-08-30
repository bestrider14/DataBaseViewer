#include "mainWindow.h"
#include "dialogConnectionSettings.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_messageBox->hide();

    ui->connectBtn->setEnabled(false);

    //statusBar()->addPermanentWidget(m_state,0);

    auto profilesInfo = m_profileStore->getProfilesNameAndUuid();

    if(!profilesInfo.empty())
    {
        for(const auto &tuple : profilesInfo)
            ui->profilesListComboBox->addItem(std::get<0>(tuple), std::get<1>(tuple));

        ui->connectBtn->setEnabled(true);
        ui->deleteProfileBtn->setEnabled(true);
        ui->profilesListComboBox->setEnabled(true);
    }
    else
    {
        ui->profilesListComboBox->setPlaceholderText("Empty");
        ui->profilesListComboBox->setEnabled(false);
    }

    onProfileSelecteChanged(ui->profilesListComboBox->currentIndex());

    connect(ui->addProfileBtn,    &QPushButton::clicked, this, &MainWindow::onAddProfileClicked);
    connect(ui->deleteProfileBtn, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
    connect(ui->connectBtn,       &QPushButton::clicked, this, &MainWindow::onConnectClicked);

    connect(m_profileStore, &ConnectionProfileStore::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
    connect(m_profileStore, &ConnectionProfileStore::profileLoaded, this, &MainWindow::onProfileLoaded);
    connect(m_profileStore, &ConnectionProfileStore::profileErased, this, &MainWindow::onProfileErased);
    connect(m_profileStore, &ConnectionProfileStore::profileSaved, this, &MainWindow::onProfileSaved);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    connect(ui->profilesListComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onProfileSelecteChanged);

    connect(ui->addRowBtn,    &QPushButton::clicked, this, &MainWindow::onAddRowClicked);
    connect(ui->deleteRowBtn, &QPushButton::clicked, this, &MainWindow::onDeleteRowClicked);

    connect(ui->searchBarWidget, &SearchBarWidget::lineEditIsEmpty, this, &MainWindow::onResetFilter);
    connect(ui->searchBarWidget, &SearchBarWidget::searchRequested, this, &MainWindow::onSearchRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    m_profileStore->erase(ui->profilesListComboBox->currentData().toString());
}

void MainWindow::onProfileSaved(const QString &p_profileName, const QString &p_uuid)
{
    ui->profilesListComboBox->addItem(p_profileName, p_uuid);
    ui->profilesListComboBox->setCurrentIndex(ui->profilesListComboBox->count()-1);
    ui->profilesListComboBox->setEnabled(true);
    ui->deleteProfileBtn->setEnabled(true);
    ui->connectBtn->setEnabled(true);
}

void MainWindow::onProfileLoaded(const ConnectionInfo &p_connectionInfo)
{
    auto *session = new DatabaseSessionWidget(p_connectionInfo, this);

    connect(session, &DatabaseSessionWidget::columnSelected, ui->searchBarWidget, &SearchBarWidget::onColumnSelected);
    connect(session, &DatabaseSessionWidget::errorMessage, m_messageBox, &MessageDialogBoxWidget::onErrorMessage);
    connect(session, &DatabaseSessionWidget::connectionStateChanged, this, &MainWindow::onConnectionStateChange);
    connect(session, &DatabaseSessionWidget::tableSelected, this, &MainWindow::onTableSelected);
    connect(session, &DatabaseSessionWidget::failedConnection, this, &MainWindow::onFailedConnection);
    connect(session, &DatabaseSessionWidget::successfullConnection, this, &MainWindow::onSuccessfullConnection);

    session->connectDatabase();
}

void MainWindow::onProfileErased(const QString &p_uuid)
{
    if(p_uuid != ui->profilesListComboBox->currentData().toString())
    {
        m_messageBox->onErrorMessage("Error", "An error occur while erasing the profile.");
        return;
    }

    ui->profilesListComboBox->removeItem(ui->profilesListComboBox->currentIndex());

    if(m_profileStore->getProfilesNameAndUuid().empty())
    {
        ui->profilesListComboBox->setPlaceholderText("Empty");
        ui->profilesListComboBox->setEnabled(false);
        ui->connectBtn->setEnabled(false);
        ui->deleteProfileBtn->setEnabled(false);
    }
}

void MainWindow::onConnectionStateChange()
{
    auto *session = qobject_cast<DatabaseSessionWidget*>(sender());

    if(session == nullptr)
        return;

    if(!session->isConnected())
    {
        auto tab = ui->tabWidget->widget(ui->tabWidget->indexOf(session));
        tab->close();
        session->deleteLater();
    }

    updateUi(session);
}

void MainWindow::onTabChanged()
{
    auto *session = currentSession();

    auto uuid = ui->tabWidget->tabBar()->tabData(ui->tabWidget->currentIndex()).toString();
    ui->profilesListComboBox->setCurrentIndex(ui->profilesListComboBox->findData(uuid));

    updateUi(session);
}

void MainWindow::onTableSelected()
{
    auto *session = qobject_cast<DatabaseSessionWidget*>(sender());

    if(session == nullptr)
        return;

    updateUi(session);
}

void MainWindow::onProfileSelecteChanged(int p_index)
{
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        if (ui->tabWidget->tabBar()->tabData(i).toString() == ui->profilesListComboBox->itemData(p_index).toString())
        {
            ui->tabWidget->setCurrentIndex(i);
            updateUi(currentSession());
            return;
        }
    }

    updateUi(nullptr);
}

void MainWindow::onFailedConnection()
{
    auto *session = qobject_cast<DatabaseSessionWidget*>(sender());

    if(auto tab = ui->tabWidget->widget(ui->tabWidget->indexOf(session)))
        tab->close();

    session->deleteLater();
}

void MainWindow::onSuccessfullConnection(const QString &p_profileName, const QString &p_uuid)
{
    auto *session = qobject_cast<DatabaseSessionWidget*>(sender());
    auto currentIndex = ui->tabWidget->addTab(session, p_profileName);
    ui->tabWidget->tabBar()->setTabData(currentIndex, p_uuid);
    ui->tabWidget->setCurrentIndex(currentIndex);
    onTabChanged();
    updateUi(session);
}

void MainWindow::onAddProfileClicked()
{
    DialogConnectionsSettings dialog(this);

    if (dialog.exec() == QDialog::Accepted)
        m_profileStore->save(dialog.getConnectionInfo());
}

void MainWindow::onConnectClicked()
{
    if(ui->tabWidget->tabBar()->tabData(ui->tabWidget->currentIndex()).toString() != ui->profilesListComboBox->itemData(ui->profilesListComboBox->currentIndex()).toString())
    {
        m_profileStore->load(ui->profilesListComboBox->currentData().toString());
        return;
    }

    auto *session = currentSession();
    if(session->isConnected())
        session->disconnectDatabase();
}

void MainWindow::onAddRowClicked()
{
    if (auto *session = currentSession())
        session->onAddRow();
}

void MainWindow::onDeleteRowClicked()
{
    if (auto *session = currentSession())
        session->onDeleteRow();
}

void MainWindow::onResetFilter()
{
    if (auto *session = currentSession())
        session->onResetFilter();
}

void MainWindow::onSearchRequested(const int p_index, const QString &p_text)
{
    if (auto *session = currentSession())
        session->onSearchRequested(p_index, p_text);
}

// Private ----------------------------------------------------
void MainWindow::updateUi(DatabaseSessionWidget *p_session)
{
    if (p_session == nullptr)
    {
        ui->connectBtn->setText("Connect");
        ui->addRowBtn->setEnabled(false);
        ui->deleteRowBtn->setEnabled(false);
        ui->searchBarWidget->reset();
        return;
    }

    if (p_session != ui->tabWidget->currentWidget())
        return;

    ui->connectBtn->setText(p_session->isConnected() ? "Disconnect" : "Connect");

    bool isTableSelected = p_session->isTableSelected();

    ui->addRowBtn->setEnabled(isTableSelected);
    ui->deleteRowBtn->setEnabled(isTableSelected);

    auto columnSelected = p_session->getColumnSelected();

    ui->searchBarWidget->onColumnSelected(columnSelected.index, columnSelected.name);
}

DatabaseSessionWidget *MainWindow::currentSession() const
{
    return qobject_cast<DatabaseSessionWidget*>(ui->tabWidget->currentWidget());
}
