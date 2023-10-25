/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_replacewidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QHBoxLayout>
#include <QPushButton>
#include <QEvent>

#include "gtpy_lineedit.h"

#include "gtpy_icons_compat.h"
#include "gtpy_stylesheet_compat.h"

#include "gtpy_replacewidget.h"

GtpyReplaceWidget::GtpyReplaceWidget(QWidget* parent) : QWidget(parent),
    m_searchLine{nullptr},
    m_replaceLine{nullptr},
    m_searchButton{nullptr},
    m_clearButton{nullptr},
    m_backwardButton{nullptr},
    m_forwardButton{nullptr}
{
    /// main layout
    auto* mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    /// search button
    m_searchButton = new QPushButton;
    m_searchButton->setIcon(GTPY_ICON(search));
    m_searchButton->setMaximumSize(QSize(20, 20));
    m_searchButton->setFlat(true);
    m_searchButton->setToolTip(tr("Search"));

    mainLayout->addWidget(m_searchButton);

    /// clear button
    m_clearButton = new QPushButton;
    m_clearButton->setIcon(GTPY_ICON(backspaceFlipped));
    m_clearButton->setMaximumSize(QSize(20, 20));
    m_clearButton->setFlat(true);
    m_clearButton->setToolTip(tr("Clear Search"));
    m_clearButton->setVisible(false);

    mainLayout->addWidget(m_clearButton);

    /// search line
    m_searchLine = new GtpyLineEdit;
    m_searchLine->setMaximumHeight(17);
    m_searchLine->setStyleSheet(GTPY_STYLESHEET(standardLineEdit));
    m_searchLine->setVisible(false);
    m_searchLine->setPlaceholderText(tr("Search for"));
    m_searchLine->installEventFilter(this);

    mainLayout->addWidget(m_searchLine);

    /// search backward button
    m_backwardButton = new QPushButton;
    m_backwardButton->setIcon(GTPY_ICON(triangleSmallLeft));
    m_backwardButton->setToolTip(tr("Search backwards"));
    m_backwardButton->setMaximumSize(QSize(15, 20));
    m_backwardButton->setFlat(true);
    m_backwardButton->setVisible(false);

    mainLayout->addWidget(m_backwardButton);

    /// search forward button
    m_forwardButton = new QPushButton;
    m_forwardButton->setIcon(GTPY_ICON(triangleSmallRight));
    m_forwardButton->setToolTip(tr("Search forwards"));
    m_forwardButton->setMaximumSize(QSize(15, 20));
    m_forwardButton->setFlat(true);
    m_forwardButton->setVisible(false);

    mainLayout->addWidget(m_forwardButton);

    /// search line
    m_replaceLine = new GtpyLineEdit;
    m_replaceLine->setMaximumHeight(17);
    m_replaceLine->setStyleSheet(GTPY_STYLESHEET(standardLineEdit));
    m_replaceLine->setVisible(false);
    m_replaceLine->setPlaceholderText(tr("Replace by"));
    m_replaceLine->installEventFilter(this);

    mainLayout->addWidget(m_replaceLine);

    /// replace button
    m_replaceButton = new QPushButton;
    m_replaceButton->setIcon(GTPY_ICON(swap));
    m_replaceButton->setMaximumSize(QSize(20, 20));
    m_replaceButton->setFlat(true);
    m_replaceButton->setToolTip(tr("Replace"));
    m_replaceButton->installEventFilter(this);

    mainLayout->addWidget(m_replaceButton);

    mainLayout->addStretch(1);

    setLayout(mainLayout);

    /// connect serach button
    connect(m_searchButton, SIGNAL(clicked(bool)), this, SLOT(enableSearch()));

    /// connect clear button
    connect(m_clearButton, SIGNAL(clicked(bool)), this,
            SLOT(disableSearchAndReplace()));

    /// connect replace button
    connect(m_replaceButton, SIGNAL(clicked(bool)), this,
            SLOT(onReplaceClicked()));

    /// connect serach line
    connect(m_searchLine, SIGNAL(textEdited(QString)), this,
            SIGNAL(searchTextEdited(QString)));
    connect(m_searchLine, SIGNAL(textChanged(QString)), this,
            SLOT(onSearchTextChanged()));
    connect(m_searchLine, SIGNAL(returnPressed()), this,
            SIGNAL(searchLineReturnPressed()));

    /// connect replace line
    connect(m_replaceLine, SIGNAL(returnPressed()), this,
            SLOT(onReplaceClicked()));

    /// connect forward and backward button
    connect(m_backwardButton, SIGNAL(clicked(bool)), this,
            SIGNAL(backwardButtonClicked()));
    connect(m_forwardButton, SIGNAL(clicked(bool)), this,
            SIGNAL(forwardButtonClicked()));
}

QString
GtpyReplaceWidget::searchText() const
{
    return m_searchLine->text();
}

QString
GtpyReplaceWidget::replaceText() const
{
    return m_replaceLine->text();
}

void
GtpyReplaceWidget::setSearchText(const QString& text)
{
    m_searchLine->setText(text);
}

void
GtpyReplaceWidget::setReplaceText(const QString& text)
{
    m_replaceLine->setText(text);
}

bool
GtpyReplaceWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_replaceLine || obj == m_searchLine || obj == m_replaceButton)
    {
        if (event->type() == QEvent::FocusOut)
        {
            if (!m_clearButton->hasFocus())
            {
                if (m_replaceLine->text().isEmpty() &&
                        m_searchLine->text().isEmpty() &&
                        !m_replaceButton->hasFocus() &&
                        !m_searchLine->hasFocus() &&
                        !m_replaceLine->hasFocus())
                {
                    disableSearchAndReplace();
                    return true;
                }
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void
GtpyReplaceWidget::enableSearch()
{
    enableSearchComponentes();
    emit searchEnabled();
}

void
GtpyReplaceWidget::enableReplace()
{
    enableSearchComponentes();
    m_replaceLine->setVisible(true);

    if (!m_searchLine->text().isEmpty())
    {
        m_replaceLine->setFocus();
    }

    emit replaceEnabled();
}

void
GtpyReplaceWidget::disableSearchAndReplace()
{
    m_searchButton->setVisible(true);
    m_clearButton->setVisible(false);
    m_backwardButton->setVisible(false);
    m_forwardButton->setVisible(false);
    m_searchLine->setVisible(false);
    m_replaceLine->setVisible(false);
    m_searchLine->clear();
    m_replaceLine->clear();

    emit searchAndReplaceDisabled();
}

void
GtpyReplaceWidget::enableSearchComponentes()
{
    enableSearchNavigation();
    m_searchButton->setVisible(false);
    m_clearButton->setVisible(true);
    m_backwardButton->setVisible(true);
    m_forwardButton->setVisible(true);
    m_searchLine->setVisible(true);
    m_searchLine->selectAll();
    m_searchLine->setFocus();
}

void
GtpyReplaceWidget::enableSearchNavigation()
{
    bool enable = !m_searchLine->text().isEmpty();
    m_forwardButton->setEnabled(enable);
    m_backwardButton->setEnabled(enable);
}

void
GtpyReplaceWidget::onSearchTextChanged()
{
    enableSearchNavigation();
    emit searchTextChanged(m_searchLine->text());
}

void
GtpyReplaceWidget::onReplaceClicked()
{
    m_replaceLine->isVisible() ?
                emit replace(m_searchLine->text(), m_replaceLine->text()) :
                enableReplace();
}
