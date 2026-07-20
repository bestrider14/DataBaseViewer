#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogConnectionSettings.h"
#include "core/connectioninfo.h"

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

