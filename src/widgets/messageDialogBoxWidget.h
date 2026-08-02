#ifndef MESSAGEDIALOGBOXWIDGET_H
#define MESSAGEDIALOGBOXWIDGET_H

#include <QWidget>
#include <QString>
#include <QMessageBox>

class MessageDialogBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageDialogBoxWidget(QWidget *p_parent = nullptr);

public slots:
    void onErrorMessage(const QString &p_title, const QString &p_message);

private:
    QMessageBox *m_messageBox = nullptr;
};

#endif // MESSAGEDIALOGBOXWIDGET_H
