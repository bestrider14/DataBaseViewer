#ifndef SEARCHBARWIDGET_H
#define SEARCHBARWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QString>
#include <QPushButton>

class SearchBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBarWidget(QWidget *p_parent = nullptr);

signals:
    void searchRequested(const int p_index, const QString &p_text);
    void lineEditIsEmpty();

public slots:
    void onColumnSelected(const int p_index, const QString &p_column);

private slots:
    void onSearchBtnClicked();
    void onTextChanged();

private:
    QLineEdit *m_searchBar = new QLineEdit(this);
    QPushButton *m_searchBtn = new QPushButton(this);

    struct {
        QString name;
        int index = 0;
    } m_searchItemSelected;
};

#endif // SEARCHBARWIDGET_H
