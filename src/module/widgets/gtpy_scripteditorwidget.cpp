/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditorwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>

#include "gtpy_scripteditor.h"
#include "gtpy_icons_compat.h"

#include "gt_searchwidget.h"
#include "gt_stylesheets.h"

#include "gtpy_scripteditorwidget.h"

GtpyScriptEditorWidget::GtpyScriptEditorWidget(int contextId, QWidget* parent) :
    QWidget(parent),
    m_editor{nullptr},
    m_undoButton{nullptr},
    m_redoButton{nullptr},
    m_backwardButton{nullptr},
    m_forwardButton{nullptr},
    m_searchWidget{nullptr}
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

    m_searchWidget = new GtSearchWidget(this);
    topLayout->addWidget(m_searchWidget);

    m_backwardButton = new QPushButton;
    m_backwardButton->setIcon(GTPY_ICON(triangleSmallLeft));
    m_backwardButton->setToolTip(tr("Search backwards"));
    m_backwardButton->setMaximumSize(QSize(15, 20));
    m_backwardButton->setFlat(true);
    m_backwardButton->setVisible(false);

    topLayout->addWidget(m_backwardButton);

    m_forwardButton = new QPushButton;
    m_forwardButton->setIcon(GTPY_ICON(triangleSmallRight));
    m_forwardButton->setToolTip(tr("Search forwards"));
    m_forwardButton->setMaximumSize(QSize(15, 20));
    m_forwardButton->setFlat(true);
    m_forwardButton->setVisible(false);

    topLayout->addWidget(m_forwardButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(editorLayout);

    setLayout(mainLayout);

    /// conect editor
    connect(m_editor, SIGNAL(redoAvailable(bool)), this,
            SLOT(setRedoButtonVisible(bool)));
    connect(m_editor, SIGNAL(undoAvailable(bool)), this,
            SLOT(setUndoButtonVisible(bool)));
    connect(m_editor, SIGNAL(searchShortcutTriggered(QString)), this,
            SLOT(setSearchedText(QString)));

    /// connect redo/undo button
    connect(m_redoButton, SIGNAL(clicked(bool)), m_editor, SLOT(redo()));
    connect(m_undoButton, SIGNAL(clicked(bool)), m_editor, SLOT(undo()));

    /// connect search widget
    connect(m_searchWidget, SIGNAL(textEdited(QString)), m_editor,
            SLOT(searchHighlighting(QString)));
    connect(m_searchWidget, SIGNAL(textEdited(QString)), this,
            SLOT(onSearchTextEdit()));
    connect(m_searchWidget, SIGNAL(searchEnabled()), this,
            SLOT(onSearchEnabled()));
    connect(m_searchWidget, SIGNAL(searchDisabled()), this,
            SLOT(onSearchDisabled()));
    connect(m_searchWidget, SIGNAL(returnPressed()), this,
            SLOT(onSearchForward()));
    connect(m_backwardButton, SIGNAL(clicked(bool)), this,
            SLOT(onSearchBackward()));
    connect(m_forwardButton, SIGNAL(clicked(bool)), this,
            SLOT(onSearchForward()));
}

GtpyScriptEditor*
GtpyScriptEditorWidget::editor() const
{
    return m_editor;
}

void
GtpyScriptEditorWidget::enableSearchNavigation(bool enable)
{
    m_forwardButton->setEnabled(enable);
    m_backwardButton->setEnabled(enable);
}

void
GtpyScriptEditorWidget::setRedoButtonVisible(bool visible)
{
//    m_redoButton->setVisible(visible);
    m_redoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::setUndoButtonVisible(bool visible)
{
//    m_undoButton->setVisible(visible);
    m_undoButton->setEnabled(visible);
}

void
GtpyScriptEditorWidget::onSearchEnabled()
{
    m_forwardButton->setVisible(true);
    m_backwardButton->setVisible(true);
    enableSearchNavigation(!m_searchWidget->text().isEmpty());
    m_editor->textSearchingActivated();
}

void
GtpyScriptEditorWidget::onSearchDisabled()
{
    m_forwardButton->setVisible(false);
    m_backwardButton->setVisible(false);
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
GtpyScriptEditorWidget::onSearchTextEdit()
{
    enableSearchNavigation(!m_searchWidget->text().isEmpty());
}

void
GtpyScriptEditorWidget::setSearchedText(const QString& text)
{
    m_editor->searchAndReplace(text, "hello", true);
    m_searchWidget->setText(text);
    m_searchWidget->enableSearch();
}
