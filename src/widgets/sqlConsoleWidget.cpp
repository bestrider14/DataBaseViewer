#include "sqlConsoleWidget.h"

SqlConsoleWidget::SqlConsoleWidget(QWidget *parent) : QWidget{parent}
{
    m_executeBtn->setText("Execute (Ctrl+Enter)");
    m_executeBtn->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));

    m_queryEdit->setPlaceholderText("-- Write your SQL query here...");

    auto *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_queryEdit);
    splitter->addWidget(m_resultView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_executeBtn, 0, Qt::AlignRight);
    layout->addWidget(splitter);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(m_executeBtn, &QPushButton::clicked, this, &SqlConsoleWidget::onExecuteClicked);

    auto *shortcut = new QShortcut(QKeySequence("Ctrl+Return"), this);
    connect(shortcut, &QShortcut::activated, this, &SqlConsoleWidget::onExecuteClicked);

    auto *shortcutHistoryUp = new QShortcut(QKeySequence("Ctrl+UP"), this);
    connect(shortcutHistoryUp, &QShortcut::activated, this, &SqlConsoleWidget::onUpHistory);

    auto *shortcutHistoryDown = new QShortcut(QKeySequence("Ctrl+DOWN"), this);
    connect(shortcutHistoryDown, &QShortcut::activated, this, &SqlConsoleWidget::onDownHistory);

    connect(m_queryEdit, &QPlainTextEdit::textChanged, this, &SqlConsoleWidget::onTextChanged);
}

void SqlConsoleWidget::clear()
{
    delete m_model;
    m_model = nullptr;
}

void SqlConsoleWidget::onSelectedQueryExecuted(QSqlQueryModel *p_model)
{
    delete m_model;
    m_model = p_model;
    m_model->setParent(this);
    m_resultView->setModel(m_model);
}

void SqlConsoleWidget::onExecuteClicked()
{
    m_queryHistory.append(m_queryEdit->toPlainText());
    m_queryHistoryIndex = m_queryHistory.count();
    emit executeRequested(m_queryEdit->toPlainText());
}

void SqlConsoleWidget::onUpHistory()
{
    if(m_queryHistoryIndex == 0) return;
    m_queryHistoryIndex--;
    m_queryEdit->setPlainText(m_queryHistory[m_queryHistoryIndex]);

    m_queryEdit->moveCursor(QTextCursor::End);
}

void SqlConsoleWidget::onDownHistory()
{
    if(m_queryHistoryIndex >= m_queryHistory.count()) return;
    m_queryHistoryIndex++;

    if(m_queryHistoryIndex != m_queryHistory.count())
        m_queryEdit->setPlainText(m_queryHistory[m_queryHistoryIndex]);
    else
        m_queryEdit->setPlainText(m_queryCurrent);

    m_queryEdit->moveCursor(QTextCursor::End);
}

void SqlConsoleWidget::onTextChanged()
{
    if(m_queryHistoryIndex == m_queryHistory.count())
        m_queryCurrent = m_queryEdit->toPlainText();
}

