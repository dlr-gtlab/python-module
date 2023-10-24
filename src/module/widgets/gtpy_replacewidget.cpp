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
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

#include "gt_lineedit.h"
#include "gt_stylesheets.h"

#include "gtpy_icons_compat.h"

#include "gtpy_replacewidget.h"

GtpyReplaceWidget::GtpyReplaceWidget(QWidget* parent) : QWidget(parent),
    m_searchLine{nullptr},
    m_replaceLine{nullptr},
    m_searchButton{nullptr},
    m_clearButton{nullptr},
    m_backwardButton{nullptr},
    m_forwardButton{nullptr}
{
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
    m_searchLine = new GtLineEdit;
    m_searchLine->setMaximumHeight(17);
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    m_searchLine->setStyleSheet(gt::gui::stylesheet::standardLineEdit());
#else
    m_searchLine->setStyleSheet(GtStyleSheets::standardLineEdit());
#endif
    m_searchLine->setVisible(false);

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
    m_replaceLine = new GtLineEdit;
    m_replaceLine->setMaximumHeight(17);
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    m_searchLine->setStyleSheet(gt::gui::stylesheet::standardLineEdit());
#else
    m_searchLine->setStyleSheet(GtStyleSheets::standardLineEdit());
#endif
    m_replaceLine->setVisible(false);

    mainLayout->addWidget(m_replaceLine);

    /// replace button
    m_replaceButton = new QPushButton;
    m_replaceButton->setIcon(GTPY_ICON(swap));
    m_replaceButton->setMaximumSize(QSize(20, 20));
    m_replaceButton->setFlat(true);
    m_replaceButton->setToolTip(tr("Replace"));
    mainLayout->addWidget(m_replaceButton);

    mainLayout->addStretch(1);

    setLayout(mainLayout);

    connect(m_searchButton, SIGNAL(clicked(bool)), this, SLOT(enableSearch()));
    connect(m_clearButton, SIGNAL(clicked(bool)), this,
            SLOT(disableSearchAndReplace()));
    connect(m_replaceButton, SIGNAL(clicked(bool)), this,
            SLOT(enableReplace()));
    connect(m_searchLine, SIGNAL(textEdited(QString)), this,
            SIGNAL(searchTextEdited(QString)));
    connect(m_searchLine, SIGNAL(textChanged(QString)), this,
            SLOT(onSearchTextChanged()));
    connect(m_searchLine, SIGNAL(returnPressed()), this,
            SIGNAL(searchLineReturnPressed()));
    connect(m_searchLine, SIGNAL(clearFocusOut()), this,
            SLOT(onSearchLineFocusOut()));
    connect(m_replaceLine, SIGNAL(clearFocusOut()), this,
            SLOT(onReplaceLineFocusOut()));

    connect(m_backwardButton, SIGNAL(clicked(bool)), this,
            SIGNAL(backwardButtonClicked()));
    connect(m_forwardButton, SIGNAL(clicked(bool)), this,
            SIGNAL(forwardButtonClicked()));
}

QString
GtpyReplaceWidget::searchText()
{
    return m_searchLine->text();
}

void
GtpyReplaceWidget::setSearchText(const QString& text)
{
    m_searchLine->setText(text);
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

    emit replaceEnabled();
}

void
GtpyReplaceWidget::disableSearchAndReplace()
{
    m_searchButton->setVisible(true);
    m_clearButton->setVisible(false);
    m_backwardButton->setVisible(false);
    m_forwardButton->setVisible(false);
    m_searchLine->clear();
    m_searchLine->setVisible(false);
    m_replaceLine->setVisible(false);

//    connect(m_replaceButton, SIGNAL(clicked(bool)), this,
//            SLOT(enableReplace()));

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
GtpyReplaceWidget::onSearchLineFocusOut()
{
    if (!m_replaceLine->hasFocus() && m_replaceLine->text().isEmpty() &&
            m_searchLine->text().isEmpty())
    {
        disableSearchAndReplace();
    }
}

void
GtpyReplaceWidget::onReplaceLineFocusOut()
{
    if (!m_searchLine->hasFocus() && m_searchLine->text().isEmpty() &&
            m_replaceLine->text().isEmpty())
    {
        disableSearchAndReplace();
    }
}

