#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QTreeView>
#include <QTreeWidgetItem>
#include <QSqlTableModel>

#include "core/databaseConnection.h"
#include "ui_mainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void initDatabaseConnection(const ConnectionInfo &p_connectionInfo);
    void stateChanged(const QString &p_newState);
    void updateTablesTree(const QStringList &p_tableList);

signals:
    void tableSelected(const QString &p_tableName);

private slots:
    void onConnectionSettingsClicked();
    void onConnectClicked();
    void onDatabaseDisconnected();
    void onDatabaseConnected();
    void receivedStatus(const QString &p_message, int p_timeout = 0);

    void onTreeItemClicked(const QTreeWidgetItem *p_item);   // extrait le nom, émet tableSelected
    void onTableSelected(const QString &p_tableName);         // récupère le modèle, met à jour la vue

private:
    Ui::MainWindow *ui;
    std::unique_ptr<DatabaseConnection> m_databaseConnection;

    bool m_isConnected = false;
    QLabel *m_state = new QLabel("Disconnected");
    QSqlTableModel *m_model = nullptr;
};
#endif // MAINWINDOW_H
