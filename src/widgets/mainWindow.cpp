#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "dialogConnectionSettings.h"
#include "core/connectionInfos.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectionSettingsBtn, &QPushButton::clicked, this, &MainWindow::onConnectionSettingsClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectionSettingsClicked()
{
    DialogConnectionsSettings dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        ConnectionInfo connectionInfo = dialog.getConnectionInfo();
    }
}

