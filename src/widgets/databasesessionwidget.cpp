#include "databasesessionwidget.h"

DatabaseSessionWidget::DatabaseSessionWidget(const ConnectionInfo &p_connectionInfo, QWidget *p_parent) : QWidget{p_parent}
{
    m_databaseConnection = new DatabaseConnection(p_connectionInfo, this);

    auto *splitter = new QSplitter(this);
    splitter->addWidget(m_explorer);
    splitter->addWidget(m_data);
    splitter->setStretchFactor(1, 3);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(m_explorer, &TableExplorerWidget::tableSelected, m_data, &TableDataWidget::showTable);
    connect(m_explorer, &TableExplorerWidget::tableSelected, this, [this]() { emit tableSelected(); });
    connect(m_databaseConnection, &DatabaseConnection::disconnected, this, &DatabaseSessionWidget::onDisconnected);
    connect(m_databaseConnection, &DatabaseConnection::failedConnection, this, &DatabaseSessionWidget::onFailedConnection);
    connect(m_databaseConnection, &DatabaseConnection::successfullConnection, this, &DatabaseSessionWidget::onSuccessfullConnection);
    connect(m_databaseConnection, &DatabaseConnection::errorMessage, this, [this](const QString &p_title, const QString &p_message){ emit errorMessage(p_title, p_message); });
    connect(m_data, &TableDataWidget::errorMessage, this, [this](const QString &p_title, const QString &p_message){ emit errorMessage(p_title, p_message); });
    connect(m_data, &TableDataWidget::columnSelected, this, &DatabaseSessionWidget::onColumnSelected);

    m_data->setConnection(m_databaseConnection);
}

void DatabaseSessionWidget::connectDatabase()
{
    m_databaseConnection->connect();
    emit connectionStateChanged();
}

void DatabaseSessionWidget::disconnectDatabase()
{
    m_data->clear();
    m_explorer->clear();
    m_databaseConnection->disconnect();
    emit connectionStateChanged();
}

void DatabaseSessionWidget::onColumnSelected(const int p_index, const QString &p_column)
{
    m_columnSelected.index = p_index;
    m_columnSelected.name = p_column;
    emit columnSelected(p_index, p_column);
}

void DatabaseSessionWidget::onSuccessfullConnection(const QString &p_profileName, const QString &p_uuid)
{
    auto tableList = m_databaseConnection->getTablesList();
    m_explorer->setTables(tableList);
    emit connectionStateChanged();
    emit successfullConnection(p_profileName, p_uuid);
}

void DatabaseSessionWidget::onDisconnected()
{
    emit connectionStateChanged();
}
