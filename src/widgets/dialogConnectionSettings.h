#ifndef DIALOGCONNECTIONSETTINGS_H
#define DIALOGCONNECTIONSETTINGS_H

#include <QDialog>

#include "core/connectioninfo.h"

namespace Ui {
class DialogConnectionsSettings;
}

class DialogConnectionsSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnectionsSettings(QWidget *parent = nullptr);
    ~DialogConnectionsSettings();

    ConnectionInfo GetConnectionInfo();

private slots:
    void OnEngineComboChanged();
    void OnEdit();

private:
    Ui::DialogConnectionsSettings *ui;

    void setOkButtonEnable(bool b);
};

#endif // DIALOGCONNECTIONSETTINGS_H
