#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>

#include "ui_mainWindow.h"
#include "core/connectionProfileStore.h"

#include "widgets/messageDialogBoxWidget.h"
#include "widgets/databasesessionwidget.h"


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
    void updateUi(DatabaseSessionWidget *p_session);
    DatabaseSessionWidget *currentSession() const;

private slots:
    void onAddProfileClicked();
    void onConnectClicked();
    void onReceivedStatus(const QString &p_message, int p_timeout = 500);
    void onCancel();
    void onDeleteProfileClicked();
    void onProfileSaved(const QString &p_profileName, const QString &p_uuid);
    void onProfileLoaded(const ConnectionInfo &p_connectionInfo);
    void onProfileErased(const QString &p_uuid);
    void onConnectionStateChange();
    void onTabChanged();
    void onTableSelected();
    void onProfileSelecteChanged(int p_index);
    void onFailedConnection();
    void onSuccessfullConnection(const QString &p_profileName, const QString &p_uuid);

    void onAddRowClicked();
    void onDeleteRowClicked();
    void onResetFilter();
    void onSearchRequested(const int p_index, const QString &p_text);

private:
    Ui::MainWindow *ui;
    MessageDialogBoxWidget *m_messageBox = new MessageDialogBoxWidget(this);
    ConnectionProfileStore *m_profileStore = new ConnectionProfileStore(this);
};
#endif // MAINWINDOW_H
