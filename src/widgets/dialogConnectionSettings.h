#ifndef DIALOGCONNECTIONSETTINGS_H
#define DIALOGCONNECTIONSETTINGS_H

#include <QDialog>

#include "core/connectionInfos.h"

namespace Ui {
class DialogConnectionsSettings;
}

class DialogConnectionsSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnectionsSettings(QWidget *parent = nullptr);
    ~DialogConnectionsSettings();

    ConnectionInfo getConnectionInfo() const;

private slots:
    void onEngineComboChanged();
    void onEdit();

private:
    Ui::DialogConnectionsSettings *ui;

    void setOkButtonEnable(bool b);
};

#endif // DIALOGCONNECTIONSETTINGS_H
