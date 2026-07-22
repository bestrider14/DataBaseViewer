#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <optional>

#include "core/databaseConnection.h"
#include "ui_mainWindow.h"
#include "dialogConnectionSettings.h"

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

private slots:
    void onConnectionSettingsClicked();
    void onConnectClicked();

private:
    Ui::MainWindow *ui;
    std::optional<DatabaseConnection> m_databaseConnection;
};
#endif // MAINWINDOW_H
