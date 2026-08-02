#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>

#include "ui_mainWindow.h"
#include "core/databaseConnection.h"

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
    void initDatabaseConnection(const ConnectionInfo &p_connectionInfo);
    void initUi();
    void stateChanged(const QString &p_newState);

private slots:
    void onConnectionSettingsClicked();
    void onConnectClicked();
    void onTableSelected();
    void onRowSelected();
    void onNoRowSelected();
    void onDatabaseDisconnected();
    void onDatabaseConnected();
    void onAddingRow();
    void receivedStatus(const QString &p_message, int p_timeout = 0);
    void onCancel();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<DatabaseConnection> m_databaseConnection;
    bool m_isConnected = false;
    QLabel *m_state = new QLabel("Disconnected");
    MessageDialogBoxWidget *m_messageBox = new MessageDialogBoxWidget(this);
};
#endif // MAINWINDOW_H
