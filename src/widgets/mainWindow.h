#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>

#include <optional>

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
    void initDatabaseConnection(const ConnectionInfo& p_connectionInfo);
    void stateChanged(const QString& p_newState);

private slots:
    void onConnectionSettingsClicked();
    void onConnectClicked();
    void receivedStatus(const QString& p_message, int p_timeout = 0);

private:
    Ui::MainWindow *ui;
    std::optional<DatabaseConnection> m_databaseConnection;

    QLabel *state = new QLabel("Disconected");
};
#endif // MAINWINDOW_H
