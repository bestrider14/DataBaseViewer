#include "tableDataWidget.h"

TableDataWidget::TableDataWidget(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this); // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    boxLayout->addWidget(m_view);
    boxLayout->setContentsMargins(0,0,0,0);

    connect(m_view, &QTableView::clicked, this, &TableDataWidget::onClick);

    QHeaderView *header = m_view->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, this, &TableDataWidget::onHeaderClicked);

}

TableDataWidget::~TableDataWidget()
{
    delete m_model;
}

void TableDataWidget::setConnection(DatabaseConnection *p_connection)
{
    m_connection = p_connection;
}

void TableDataWidget::showTable(const QString &p_tableName)
{
    delete m_model;
    m_model = m_connection->getTableData(p_tableName);

    // Choix de OnFieldChange justifié dans le README (section "Décisions de conception").
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_view->setSortingEnabled(true);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_view->setModel(m_proxyModel);
    m_proxyModel->setSourceModel(m_model);

    m_view->show();

    connect(m_model, &CustomTableModel::editFailed, this, &TableDataWidget::onEditFailed);
}

void TableDataWidget::clear()
{
    delete m_model;
    m_view->clearSpans();
    m_view->setModel(nullptr);
    m_model = nullptr;
    m_proxyModel = nullptr;
}

void TableDataWidget::onEditFailed(const QSqlError &p_error)
{
    emit error("Edit Failed", p_error.text());
}

void TableDataWidget::onClick(const QModelIndex &p_index)
{
    if(p_index.isValid() && m_view->selectionModel()->hasSelection())
        emit rowSelected();
    else
        emit noRowSelected();
}

void TableDataWidget::onHeaderClicked(const int p_index)
{
    emit columnSelected(p_index ,m_model->headerData(p_index, Qt::Horizontal).toString());
}

void TableDataWidget::onAddRow()
{
    if(m_model->insertRow(m_model->rowCount()))
        emit addingRow();
    else
        emit error("Adding a row failed", "Something wrong append");
}

void TableDataWidget::onDeletingRow()
{
    auto reply = QMessageBox::question(this, "Delete confirmation", "Are you sure to delete this data", QMessageBox::Ok | QMessageBox::Cancel);

    if(reply != QMessageBox::Ok)
        return;

    auto indexList = m_view->selectionModel()->selectedRows();

    std::sort(indexList.begin(), indexList.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    for (const auto &index : indexList)
        m_model->removeRows(m_proxyModel->mapToSource(index).row(), 1);


    if(!m_model->submitAll())
    {
        emit error("Error on submit", m_model->lastError().text() + " All change will be reverted.");
        m_model->revertAll();
    }

    m_model->select();
    emit noRowSelected();
}

void TableDataWidget::onCancel()
{
    m_model->revertAll();
    emit canceled();
    emit noRowSelected();
}

void TableDataWidget::onSearchRequested(const int p_index, const QString &p_text)
{
    m_proxyModel->setFilterRegularExpression(QRegularExpression::escape(p_text));
    m_proxyModel->setFilterKeyColumn(p_index);
}

void TableDataWidget::resetFilter()
{
    m_proxyModel->setFilterRegularExpression("");
}
