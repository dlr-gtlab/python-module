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
    m_redoButton{nullptr}
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

    auto* replaceWidget = new GtpyReplaceWidget(this);
    topLayout->addWidget(replaceWidget);

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
    connect(m_scriptView, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_scriptView, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(enableSearch()));
    connect(m_scriptView, SIGNAL(replaceShortcutTriggered(QString)), replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_scriptView, SIGNAL(replaceShortcutTriggered(QString)), replaceWidget,
            SLOT(enableReplace()));

    /// connect redo/undo button
    connect(m_redoButton, SIGNAL(clicked(bool)), m_scriptView, SLOT(redo()));
    connect(m_undoButton, SIGNAL(clicked(bool)), m_scriptView, SLOT(undo()));

    /// connect serach and replace
//    connect(replaceWidget, SIGNAL(searchTextChanged(QString)), m_scriptView,
//            SLOT(searchHighlighting(QString)));
    connect(replaceWidget, SIGNAL(searchTextChanged(QString)), m_scriptView,
            SLOT(setTextToHighlight(QString)));

//    connect(replaceWidget, SIGNAL(searchEnabled()), this,
//            SLOT(onSearchEnabled()));
    connect(replaceWidget, SIGNAL(searchAndReplaceDisabled()), this,
            SLOT(onSearchDisabled()));
    connect(replaceWidget, SIGNAL(searchLineReturnPressed()), this,
            SLOT(onSearchForward()));
    connect(replaceWidget, SIGNAL(backwardButtonClicked()), this,
            SLOT(onSearchBackward()));
    connect(replaceWidget, SIGNAL(forwardButtonClicked()), this,
            SLOT(onSearchForward()));

    connect(replaceWidget, SIGNAL(replace(QString, QString)), this,
            SLOT(onReplace(QString, QString)));

}

GtpyScriptView*
GtpyScriptEditorWidget::scriptView() const
{
    return m_scriptView;
}

void
GtpyScriptEditorWidget::setScript(const QString& script)
{
    m_scriptView->setScript(script);
}

void
GtpyScriptEditorWidget::setRedoButtonEnabled(bool visible)
{
    m_redoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::setUndoButtonEnabled(bool visible)
{
    m_undoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::onSearchEnabled()
{
//    m_scriptView->textSearchingActivated();
}

void
GtpyScriptEditorWidget::onSearchDisabled()
{
//    m_scriptView->removeSearchHighlighting();
}

void
GtpyScriptEditorWidget::onSearchBackward()
{
    m_scriptView->moveCursorToLastFound();
}

void
GtpyScriptEditorWidget::onSearchForward()
{
    m_scriptView->moveCursorToNextFound();
}

void
GtpyScriptEditorWidget::onReplace(const QString& find, const QString& replaceBy)
{
    m_scriptView->searchAndReplace(find, replaceBy, false);
    m_scriptView->setTextToHighlight(find);
}
