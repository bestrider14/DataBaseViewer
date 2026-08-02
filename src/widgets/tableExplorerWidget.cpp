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

void TableExplorerWidget::setTables(const QStringList &p_tableList)
{
    for (const QString &table : p_tableList)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, table);
        m_tree->addTopLevelItem(item);
    }
}

void TableExplorerWidget::clear()
{
    m_tree->clear();
}

void TableExplorerWidget::onItemClicked(const QTreeWidgetItem *p_item)
{
    emit tableSelected(p_item->text(0));
}
