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

#include "gtpy_scripteditor.h"
#include "gtpy_replacewidget.h"
#include "gtpy_icons_compat.h"

#include "gtpy_scripteditorwidget.h"

GtpyScriptEditorWidget::GtpyScriptEditorWidget(int contextId, QWidget* parent) :
    QWidget(parent),
    m_editor{nullptr},
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
    m_editor = new GtpyScriptEditor(contextId);
    m_editor->setStyleSheet("QPlainTextEdit {  border: 0px; }");

    QTextOption defaultOps = m_editor->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces);
    m_editor->document()->setDefaultTextOption(defaultOps);

    auto* highlighter = new GtPyHighlighter(m_editor->document());

    Q_UNUSED(highlighter)

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_editor);

    setLayout(mainLayout);

    /// conect editor
    connect(m_editor, SIGNAL(redoAvailable(bool)), this,
            SLOT(setRedoButtonEnabled(bool)));
    connect(m_editor, SIGNAL(undoAvailable(bool)), this,
            SLOT(setUndoButtonEnabled(bool)));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(enableSearch()));
    connect(m_editor, SIGNAL(replaceShortcutTriggered(QString)), replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_editor, SIGNAL(replaceShortcutTriggered(QString)), replaceWidget,
            SLOT(enableReplace()));

    /// connect redo/undo button
    connect(m_redoButton, SIGNAL(clicked(bool)), m_editor, SLOT(redo()));
    connect(m_undoButton, SIGNAL(clicked(bool)), m_editor, SLOT(undo()));

    /// connect serach and replace
    connect(replaceWidget, SIGNAL(searchTextChanged(QString)), m_editor,
            SLOT(searchHighlighting(QString)));
    connect(replaceWidget, SIGNAL(searchEnabled()), this,
            SLOT(onSearchEnabled()));
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

GtpyScriptEditor*
GtpyScriptEditorWidget::editor() const
{
    return m_editor;
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
    m_editor->textSearchingActivated();
}

void
GtpyScriptEditorWidget::onSearchDisabled()
{
    m_editor->removeSearchHighlighting();
}

void
GtpyScriptEditorWidget::onSearchBackward()
{
    m_editor->moveCursorToLastFound();
}

void
GtpyScriptEditorWidget::onSearchForward()
{
    m_editor->moveCursorToNextFound();
}

void
GtpyScriptEditorWidget::onReplace(const QString& find, const QString& replaceBy)
{
    m_editor->searchAndReplace(find, replaceBy, false);
    m_editor->searchHighlighting(find);
}
