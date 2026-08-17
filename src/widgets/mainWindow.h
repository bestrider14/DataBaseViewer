#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>

#include "ui_mainWindow.h"
#include "core/databaseConnection.h"
#include "core/connectionProfileStore.h"

#include "widgets/messageDialogBoxWidget.h"

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
    void initUi();
    void stateChanged(const QString &p_newState);
    void setConnection(const QString &p_selectedProfile);

private slots:
    void onAddProfileClicked();
    void onConnectClicked();
    void onTableSelected(const QString &p_tableName);
    void onRowSelected();
    void onNoRowSelected();
    void onDatabaseDisconnected();
    void onDatabaseConnected();
    void onAddingRow();
    void receivedStatus(const QString &p_message, int p_timeout = 0);
    void onCancel();
    void onDeleteProfileClicked();
    void onComboBoxChanged(int p_index);
    void onProfileSaved(const QString &p_profileName, const QString &p_uuid);
    void onProfileLoaded(const QString &p_uuid, const ConnectionInfo &p_connectionInfo);
    void onProfileErased(const QString &p_uuid);

private:
    Ui::MainWindow *ui;
    std::unordered_map<QString, std::unique_ptr<DatabaseConnection>> m_databaseConnections;
    std::unordered_map<QString, QString> m_tableSelectedList;
    QLabel *m_state = new QLabel("Disconnected");
    MessageDialogBoxWidget *m_messageBox = new MessageDialogBoxWidget(this);
    ConnectionProfileStore *m_profiles = new ConnectionProfileStore(this);
};
#endif // MAINWINDOW_H
