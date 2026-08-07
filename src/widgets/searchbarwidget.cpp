#include "searchbarwidget.h"

SearchBarWidget::SearchBarWidget(QWidget *parent) : QWidget{parent}
{
    m_searchBtn->setText("Search");
    m_searchBar->setPlaceholderText("Search in ...");

    QHBoxLayout *boxLayout = new QHBoxLayout(this); // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    boxLayout->addWidget(m_searchBtn);
    boxLayout->addWidget(m_searchBar);
    boxLayout->setContentsMargins(0,0,0,0);

    connect(m_searchBtn, &QPushButton::clicked, this, &SearchBarWidget::onSearchBtnClicked);
    connect(m_searchBar, &QLineEdit::textChanged, this, &SearchBarWidget::onTextChanged);
}

void SearchBarWidget::onColumnSelected(const int p_index, const QString &p_name)
{
    m_searchItemSelected.name = p_name;
    m_searchItemSelected.index = p_index;
    m_searchBar->setPlaceholderText("Search in " + m_searchItemSelected.name);
}

void SearchBarWidget::onSearchBtnClicked()
{
    emit searchRequested(m_searchItemSelected.index, m_searchBar->text());
}

void SearchBarWidget::onTextChanged()
{
    if(m_searchBar->text().isEmpty())
        emit lineEditIsEmpty();
}
