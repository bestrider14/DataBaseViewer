#include "customTableModel.h"

CustomTableModel::CustomTableModel(QObject *p_parent, QSqlDatabase p_db) : QSqlTableModel(p_parent, p_db) {}

bool CustomTableModel::setData(const QModelIndex &p_index, const QVariant &p_value, int p_role)
{
    bool ok = QSqlTableModel::setData(p_index, p_value, p_role);

    if (!ok)
        emit editFailed(lastError());

    return ok;
}
