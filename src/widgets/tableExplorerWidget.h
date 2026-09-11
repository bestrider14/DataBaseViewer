#ifndef TABLEEXPLORERWIDGET_H
#define TABLEEXPLORERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

class TableExplorerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableExplorerWidget(QWidget *parent = nullptr);

    void setTables(const QMap<QString, QStringList> &p_map);
    bool isTableSelected();
    void clear();

signals:
    void tableSelected(const QString &p_SchemaName, const QString &p_tableName);

private slots:
    void onItemClicked(const QTreeWidgetItem *p_item);

private:
    QTreeWidget *m_tree;
};

#endif // TABLEEXPLORERWIDGET_H
