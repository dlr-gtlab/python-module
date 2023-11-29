/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditorwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "gt_pyhighlighter.h"

#include "gtpy_scriptview.h"
#include "gtpy_replacewidget.h"
#include "gtpy_icons_compat.h"

#include "gtpy_scripteditorwidget.h"

GtpyScriptEditorWidget::GtpyScriptEditorWidget(int contextId, QWidget* parent) :
    QWidget(parent),
    m_scriptView{nullptr},
    m_undoButton{nullptr},
    m_redoButton{nullptr},
    m_replaceWidget{nullptr}
{
    ///  main layout
    auto* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);

    /// top layout
    auto* topLayout = new QHBoxLayout();
    topLayout->setSpacing(0);
    topLayout->setMargin(0);
    topLayout->setContentsMargins(0, 0, 0, 0);

    m_undoButton = new QPushButton;
    m_undoButton->setIcon(GTPY_ICON(undo));
    m_undoButton->setToolTip(tr("Undo"));
    m_undoButton->setFlat(true);
    m_undoButton->setEnabled(false);

    topLayout->addWidget(m_undoButton);

    m_redoButton = new QPushButton;
    m_redoButton->setIcon(GTPY_ICON(redo));
    m_redoButton->setToolTip(tr("Redo"));
    m_redoButton->setFlat(true);
    m_redoButton->setEnabled(false);

    topLayout->addWidget(m_redoButton);

    topLayout->addStretch(1);

    m_replaceWidget = new GtpyReplaceWidget(this);
    topLayout->addWidget(m_replaceWidget);

    /// script editor
    m_scriptView = new GtpyScriptView(contextId);
    m_scriptView->setStyleSheet("QPlainTextEdit {  border: 0px; }");

    QTextOption defaultOps = m_scriptView->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces);
    m_scriptView->document()->setDefaultTextOption(defaultOps);

    auto* highlighter = new GtPyHighlighter(m_scriptView->document());
    Q_UNUSED(highlighter)

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_scriptView);

    setLayout(mainLayout);

    /// conect editor
    connect(m_scriptView, SIGNAL(redoAvailable(bool)), this,
            SLOT(setRedoButtonEnabled(bool)));
    connect(m_scriptView, SIGNAL(undoAvailable(bool)), this,
            SLOT(setUndoButtonEnabled(bool)));
    connect(m_scriptView, SIGNAL(searchShortcutTriggered(QString)), m_replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_scriptView, SIGNAL(searchShortcutTriggered(QString)), m_replaceWidget,
            SLOT(enableSearch()));
    connect(m_scriptView, SIGNAL(replaceShortcutTriggered(QString)), m_replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_scriptView, SIGNAL(replaceShortcutTriggered(QString)), m_replaceWidget,
            SLOT(enableReplace()));

    /// connect redo/undo button
    connect(m_redoButton, SIGNAL(clicked(bool)), m_scriptView, SLOT(redo()));
    connect(m_undoButton, SIGNAL(clicked(bool)), m_scriptView, SLOT(undo()));

    /// connect serach and replace
    connect(m_replaceWidget, SIGNAL(searchTextChanged(QString)), this,
            SLOT(onSearchTextChanged(QString)));
    connect(m_replaceWidget, SIGNAL(searchLineReturnPressed()), this,
            SLOT(onSearchForward()));
    connect(m_replaceWidget, SIGNAL(backwardButtonClicked()), this,
            SLOT(onSearchBackward()));
    connect(m_replaceWidget, SIGNAL(forwardButtonClicked()), this,
            SLOT(onSearchForward()));
    connect(m_replaceWidget, SIGNAL(replace(QString, QString)), this,
            SLOT(onReplace(QString, QString)));
}

GtpyScriptView*
GtpyScriptEditorWidget::scriptView() const
{
    return m_scriptView;
}

void
GtpyScriptEditorWidget::setScript(const QString& script) const
{
    m_scriptView->setScript(script);
}

void
GtpyScriptEditorWidget::setRedoButtonEnabled(bool visible) const
{
    m_redoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::setUndoButtonEnabled(bool visible) const
{
    m_undoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::onSearchBackward() const
{
    m_scriptView->selectNextMatch(m_replaceWidget->searchText(), true);
}

void
GtpyScriptEditorWidget::onSearchForward() const
{
    m_scriptView->selectNextMatch(m_replaceWidget->searchText());
}

void
GtpyScriptEditorWidget::onReplace(const QString& find, const QString& replaceBy) const
{
    m_scriptView->findAndReplace(find, replaceBy);
    m_scriptView->selectNextMatch(m_replaceWidget->searchText());
}

void
GtpyScriptEditorWidget::onSearchTextChanged(const QString& text) const
{
    m_scriptView->setHighlight({text, Qt::CaseSensitive});
}
