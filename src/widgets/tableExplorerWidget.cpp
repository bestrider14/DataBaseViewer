#include "tableExplorerWidget.h"

TableExplorerWidget::TableExplorerWidget(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    m_tree = new QTreeWidget();
    m_tree->setHeaderLabel("Database Tables");
    boxLayout->addWidget(m_tree);
    boxLayout->setContentsMargins(0,0,0,0);

    connect(m_tree, &QTreeWidget::itemClicked, this, &TableExplorerWidget::onItemClicked);
}

void TableExplorerWidget::setTables(const QMap<QString, QStringList> &p_map)
{
    for (auto it = p_map.constBegin(); it != p_map.constEnd(); ++it)
    {
        auto *schemaItem = new QTreeWidgetItem(m_tree, {it.key()});
        QStringList sortedTables = it.value();
        sortedTables.sort(Qt::CaseInsensitive);
        for (const QString &table : sortedTables)
            new QTreeWidgetItem(schemaItem, {table});
    }
}

bool TableExplorerWidget::isTableSelected()
{
    if(m_tree->currentItem() == nullptr || m_tree->currentItem()->parent() == nullptr)
        return false;
    return true;
}

void TableExplorerWidget::clear()
{
    m_tree->clear();
}

void TableExplorerWidget::onItemClicked(const QTreeWidgetItem *p_item)
{
    if(p_item->parent() == nullptr)
        return;

    emit tableSelected(p_item->parent()->text(0) ,p_item->text(0));
}
