/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditorwidget.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.10.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPointer>

#include "gt_pyhighlighter.h"

#include "gtpy_scriptview.h"
#include "gtpy_replacewidget.h"
#include "gtpy_icons_compat.h"

#include "gtpy_scripteditorwidget.h"

struct GtpyScriptEditorWidget::Impl
{
    /// Script view
    QPointer<GtpyScriptView> m_scriptView{nullptr};

    /// Undo button
    QPointer<QPushButton> m_undoButton{nullptr};

    /// Redo button
    QPointer<QPushButton> m_redoButton{nullptr};

    /// Replace widget
    QPointer<GtpyReplaceWidget> m_replaceWidget{nullptr};
};

GtpyScriptEditorWidget::GtpyScriptEditorWidget(int contextId, QWidget* parent) :
    QWidget(parent),
    m_pimpl{std::make_unique<GtpyScriptEditorWidget::Impl>()}
{
    ///  main layout
    auto* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);

    /// top layout
    auto* topLayout = new QHBoxLayout();
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0, 0, 0, 0);

    m_pimpl->m_undoButton = new QPushButton;
    m_pimpl->m_undoButton->setIcon(GTPY_ICON(undo));
    m_pimpl->m_undoButton->setToolTip(tr("Undo"));
    m_pimpl->m_undoButton->setFlat(true);
    m_pimpl->m_undoButton->setEnabled(false);

    topLayout->addWidget(m_pimpl->m_undoButton);

    m_pimpl->m_redoButton = new QPushButton;
    m_pimpl->m_redoButton->setIcon(GTPY_ICON(redo));
    m_pimpl->m_redoButton->setToolTip(tr("Redo"));
    m_pimpl->m_redoButton->setFlat(true);
    m_pimpl->m_redoButton->setEnabled(false);

    topLayout->addWidget(m_pimpl->m_redoButton);

    topLayout->addStretch(1);

    m_pimpl->m_replaceWidget = new GtpyReplaceWidget(this);
    topLayout->addWidget(m_pimpl->m_replaceWidget);

    /// script editor
    m_pimpl->m_scriptView = new GtpyScriptView(contextId);
    m_pimpl->m_scriptView->setStyleSheet("QPlainTextEdit {  border: 0px; }");

    QTextOption defaultOps = m_pimpl->m_scriptView->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces);
    m_pimpl->m_scriptView->document()->setDefaultTextOption(defaultOps);

    auto* highlighter = new GtPyHighlighter(m_pimpl->m_scriptView->document());
    Q_UNUSED(highlighter)

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_pimpl->m_scriptView);

    setLayout(mainLayout);

    /// conect editor
    connect(m_pimpl->m_scriptView, SIGNAL(redoAvailable(bool)), this,
            SLOT(setRedoButtonEnabled(bool)));
    connect(m_pimpl->m_scriptView, SIGNAL(undoAvailable(bool)), this,
            SLOT(setUndoButtonEnabled(bool)));

    /// connect redo/undo button
    connect(m_pimpl->m_redoButton, SIGNAL(clicked(bool)), m_pimpl->m_scriptView,
            SLOT(redo()));
    connect(m_pimpl->m_undoButton, SIGNAL(clicked(bool)), m_pimpl->m_scriptView,
            SLOT(undo()));

    /// connect serach and replace
    connect(m_pimpl->m_replaceWidget, SIGNAL(searchTextChanged(QString)), this,
            SLOT(onSearchTextChanged(QString)));
    connect(m_pimpl->m_replaceWidget, SIGNAL(searchLineReturnPressed()), this,
            SLOT(onSearchForward()));
    connect(m_pimpl->m_replaceWidget, SIGNAL(backwardButtonClicked()), this,
            SLOT(onSearchBackward()));
    connect(m_pimpl->m_replaceWidget, SIGNAL(forwardButtonClicked()), this,
            SLOT(onSearchForward()));
    connect(m_pimpl->m_replaceWidget, SIGNAL(replace(QString, QString)), this,
            SLOT(onReplace(QString, QString)));
}

GtpyScriptEditorWidget::~GtpyScriptEditorWidget() = default;

void
GtpyScriptEditorWidget::setScript(const QString& script) const
{
    m_pimpl->m_scriptView->setScript(script);
}

QString
GtpyScriptEditorWidget::script() const
{
    return m_pimpl->m_scriptView->script();
}

void
GtpyScriptEditorWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_F:
                onFindShortcut();
                break;
            case Qt::Key_R:
                onReplaceShortcut();
                break;
            case Qt::Key_E:
                emit evaluationTriggered();
                break;
            default:
                break;
        }
    }

    QWidget::keyPressEvent(event);
}

void
GtpyScriptEditorWidget::setSearchText() const
{
    if (m_pimpl->m_scriptView->hasSelection())
    {
        m_pimpl->m_replaceWidget->setSearchText(
                    m_pimpl->m_scriptView->selectedText());
    }
}

void
GtpyScriptEditorWidget::setRedoButtonEnabled(bool enable) const
{
    m_pimpl->m_redoButton->setEnabled(enable);
}

void
GtpyScriptEditorWidget::setUndoButtonEnabled(bool enable) const
{
    m_pimpl->m_undoButton->setEnabled(enable);
}

void
GtpyScriptEditorWidget::onSearchBackward() const
{
    m_pimpl->m_scriptView->selectNextMatch(
                m_pimpl->m_replaceWidget->searchText(), true);
}

void
GtpyScriptEditorWidget::onSearchForward() const
{
    m_pimpl->m_scriptView->selectNextMatch(
                m_pimpl->m_replaceWidget->searchText());
}

void
GtpyScriptEditorWidget::onReplace(const QString& find,
                                  const QString& replaceBy) const
{
    m_pimpl->m_scriptView->findAndReplace(find, replaceBy);
    m_pimpl->m_scriptView->selectNextMatch(
                m_pimpl->m_replaceWidget->searchText());
}

void
GtpyScriptEditorWidget::onSearchTextChanged(const QString& text) const
{
    m_pimpl->m_scriptView->setHighlight({text, Qt::CaseSensitive});
}

void
GtpyScriptEditorWidget::onFindShortcut() const
{
    setSearchText();
    m_pimpl->m_replaceWidget->enableSearch();
}

void
GtpyScriptEditorWidget::onReplaceShortcut() const
{
    setSearchText();
    m_pimpl->m_replaceWidget->enableReplace();
}
