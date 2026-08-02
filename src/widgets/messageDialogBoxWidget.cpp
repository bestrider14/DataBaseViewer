#include "messageDialogBoxWidget.h"

MessageDialogBoxWidget::MessageDialogBoxWidget(QWidget *p_parent) : QWidget{p_parent}
{}

void MessageDialogBoxWidget::onErrorMessage(const QString &p_title, const QString &p_message)
{
    delete m_messageBox;
    m_messageBox = new QMessageBox(QMessageBox::Critical, p_title, p_message, QMessageBox::Ok, this);
    m_messageBox->exec();
}
