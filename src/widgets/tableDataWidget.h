#ifndef TABLEDATAWIDGET_H
#define TABLEDATAWIDGET_H

#include <algorithm>
#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlError>

#include "core/customTableModel.h"
#include "core/databaseConnection.h"
#include "widgets/messageDialogBoxWidget.h"

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
    void error(const QString &p_title, const QString &p_message);
    void addingRow();
    void submited();
    void canceled();
    void rowSelected();
    void noRowSelected();

public slots:
    void onAddRow();
    void onDeletingRow();
    void onCancel();

private slots:
    void onEditFailed(const QSqlError &p_error);
    void onClick(const QModelIndex &p_index);

private:
    QTableView *m_view;
    DatabaseConnection *m_connection = nullptr;
    CustomTableModel *m_model = nullptr;
    MessageDialogBoxWidget m_messageDialog = MessageDialogBoxWidget(this);
};

#endif // TABLEDATAWIDGET_H