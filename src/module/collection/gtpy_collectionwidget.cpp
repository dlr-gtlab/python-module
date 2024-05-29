/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionwidget.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2021
 * Author: Marvin Noethen (AT-TW)
 */

#include <QHBoxLayout>
#include <QFile>
#include <QDir>
#include <QSplitter>

#include "gt_collectionitem.h"
#include "gt_pyhighlighter.h"

#include "gtpy_editorsettings.h"
#include "gtpy_scripteditor.h"
#include "gtpy_contextmanager.h"

#include "gtpy_collectionlocalwidget.h"

#include "gtpy_collectionwidget.h"

GtpyCollectionWidget::GtpyCollectionWidget(QWidget* parent) : QWidget(parent),
    m_editor(Q_NULLPTR)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;

    ///Editor layout
    m_editor = new GtpyScriptEditor(GtpyContextManager::CollectionContext,
                                    this);
    QTextOption defaultOps = m_editor->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces /*|
                            QTextOption::ShowLineAndParagraphSeparators*/);

    m_editor->document()->setDefaultTextOption(defaultOps);
    m_editor->setReadOnly(true);

    GtPyHighlighter* highlighter = new GtPyHighlighter(m_editor->document());

    m_editor->setVisible(false);

    Q_UNUSED(highlighter)

    m_defaultLocalWidget = new GtpyCollectionLocalWidget(this);

    ///Splitter
    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);

    splitter->addWidget(m_defaultLocalWidget);
    splitter->addWidget(m_editor);

    splitter->setCollapsible(splitter->indexOf(m_defaultLocalWidget), false);
    splitter->setCollapsible(splitter->indexOf(m_editor), false);

    mainLayout->addWidget(splitter);

    setLayout(mainLayout);

    connect(m_defaultLocalWidget, SIGNAL(itemSelected(QModelIndex)),
            SLOT(onItemSelected(QModelIndex)));
}

void
GtpyCollectionWidget::setItems(const QList<GtCollectionItem>& items)
{
    m_defaultLocalWidget->setItems(items);
}

void
GtpyCollectionWidget::onItemSelected(const QModelIndex& index)
{
    GtCollectionItem item = m_defaultLocalWidget->getItem(index);

    if (item.isValid())
    {
        QDir dir(item.localPath());

        QString filePath = dir.filePath(item.files().first());

        QFile script(filePath);

        if (script.exists())
        {
            if (script.open(QFile::ReadOnly))
            {
                QTextStream in(&script);

                if (m_editor)
                {
                    m_editor->setPlainText(in.readAll());
                }
            }
        }

        if (m_editor)
        {
            m_editor->setVisible(true);
        }
    }
}
