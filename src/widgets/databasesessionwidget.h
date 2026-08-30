#ifndef DATABASESESSIONWIDGET_H
#define DATABASESESSIONWIDGET_H

#include <QWidget>
#include <QSplitter>

#include "core/databaseConnection.h"
#include "widgets/tableDataWidget.h"
#include "widgets/tableExplorerWidget.h"

class DatabaseSessionWidget : public QWidget
{
    Q_OBJECT
public:

    struct ColumnInfos{
        QString name;
        int index = -1;
    };

    explicit DatabaseSessionWidget(const ConnectionInfo &p_connectionInfo, QWidget *p_parent = nullptr);

    void connectDatabase();
    void disconnectDatabase();

    inline bool isConnected() const { return m_databaseConnection->isConnected(); }
    inline bool isTableSelected() const { return m_explorer->isTableSelected(); };
    inline ColumnInfos  getColumnSelected() const { return m_columnSelected; };


public slots:
    inline void onAddRow() { m_data->onAddRow(); };
    inline void onDeleteRow() {m_data->onDeleteRow(); };
    inline void onSearchRequested(const int p_index, const QString &p_text) {m_data->onSearchRequested(p_index, p_text); };
    inline void onResetFilter() { m_data->onResetFilter(); };
    inline void onCancel() {m_data->onCancel(); };
    inline void onFailedConnection() { emit failedConnection(); };
    void onColumnSelected(const int p_index, const QString &p_column);;
    void onSuccessfullConnection(const QString &p_profileName, const QString &p_uuid);

signals:
    void tableSelected();
    void connectionStateChanged();
    void failedConnection();
    void successfullConnection(const QString &p_profileName, const QString &p_uuid);
    void errorMessage(const QString &p_title, const QString &p_message);
    void columnSelected(const int p_index, const QString &p_column);

private slots:
    void onDisconnected();

private:
    DatabaseConnection  *m_databaseConnection = nullptr;
    TableExplorerWidget *m_explorer = new TableExplorerWidget;
    TableDataWidget     *m_data = new TableDataWidget;
    ColumnInfos          m_columnSelected;
};
#endif // DATABASESESSIONWIDGET_H
