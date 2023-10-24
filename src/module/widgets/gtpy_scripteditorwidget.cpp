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

#include "gt_searchwidget.h"

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
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* editorLayout = new QVBoxLayout();

    m_editor = new GtpyScriptEditor(contextId);

    editorLayout->addWidget(m_editor);

    QHBoxLayout* topLayout = new QHBoxLayout();

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

    GtpyReplaceWidget* replaceWidget = new GtpyReplaceWidget(this);
    topLayout->addWidget(replaceWidget);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(editorLayout);

    setLayout(mainLayout);

    /// conect editor
    connect(m_editor, SIGNAL(redoAvailable(bool)), this,
            SLOT(setRedoButtonVisible(bool)));
    connect(m_editor, SIGNAL(undoAvailable(bool)), this,
            SLOT(setUndoButtonVisible(bool)));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(setSearchText(QString)));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), replaceWidget,
            SLOT(enableSearch()));

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
}

GtpyScriptEditor*
GtpyScriptEditorWidget::editor() const
{
    return m_editor;
}

void
GtpyScriptEditorWidget::setRedoButtonVisible(bool visible)
{
    m_redoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::setUndoButtonVisible(bool visible)
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
