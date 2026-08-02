#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlError>

#include "qsqltablemodel.h"


class CustomTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CustomTableModel(QObject *p_parent = nullptr, QSqlDatabase p_db = QSqlDatabase());

    bool setData(const QModelIndex &p_index, const QVariant &p_value, int p_role) override;

signals:
    void editFailed(const QSqlError &p_error);
};

#endif // CUSTOMTABLEMODEL_H
