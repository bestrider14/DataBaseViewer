#ifndef SQLCONSOLEWIDGET_H
#define SQLCONSOLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTableView>
#include <QShortcut>
#include <QSqlQueryModel>

class SqlConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SqlConsoleWidget(QWidget *p_parent = nullptr);
    void clear();

    inline QTableView *resultView() const { return m_resultView; };

public slots:
    void onSelectedQueryExecuted(QSqlQueryModel *p_model);

signals:
    void executeRequested(const QString &p_querry);

private slots:
    void onExecuteClicked();
    void onUpHistory();
    void onDownHistory();
    void onTextChanged();

private:
    QPlainTextEdit *m_queryEdit  = new QPlainTextEdit(this);
    QPushButton    *m_executeBtn = new QPushButton(this);
    QTableView     *m_resultView = new QTableView(this);

    QSqlQueryModel *m_model = nullptr;

    QString        m_queryCurrent;
    QList<QString> m_queryHistory;
    int            m_queryHistoryIndex = 0;
};

#endif // SQLCONSOLEWIDGET_H
