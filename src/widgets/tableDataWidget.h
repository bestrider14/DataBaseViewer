#ifndef TABLEDATAWIDGET_H
#define TABLEDATAWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlError>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QRegularExpression>
#include <QMessageBox>

#include "core/customTableModel.h"
#include "core/databaseConnection.h"


class TableDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableDataWidget(QWidget *parent = nullptr);
    ~TableDataWidget();

    void setConnection(DatabaseConnection *p_connection);
    void showTable(const QString &p_tableName);
    void clear();

signals:
    void errorMessage(const QString &p_title, const QString &p_message);
    void addingRow();
    void submited();
    void canceled();
    void rowSelected();
    void noRowSelected();
    void columnSelected(const int p_index, const QString &p_column);

public slots:
    void onAddRow();
    void onDeleteRow();
    void onCancel();
    void onSearchRequested(const int p_index, const QString &p_text);
    void onResetFilter();

private slots:
    void onEditFailed(const QSqlError &p_error);
    void onClick(const QModelIndex &p_index);
    void onHeaderClicked(const int p_index);

private:
    QTableView *m_view = new QTableView(this);
    DatabaseConnection *m_connection = nullptr;
    CustomTableModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxyModel = new QSortFilterProxyModel(this);
};

#endif // TABLEDATAWIDGET_H
