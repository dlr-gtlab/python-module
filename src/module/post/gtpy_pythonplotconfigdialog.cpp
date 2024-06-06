/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotconfigdialog.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (DLR AT-TWK)
 */

#include <QVBoxLayout>

#include "gtpy_contextmanager.h"
#include "gt_pyhighlighter.h"

#include "gtpy_scripteditor.h"

#include "gtpy_pythonplotconfigdialog.h"

GtpyPythonPlotConfigDialog::GtpyPythonPlotConfigDialog(QWidget* parent) :
    QDialog (parent)
{
    setWindowTitle(tr("Config Python Post Plot"));

    QVBoxLayout* layout = new QVBoxLayout;

    m_editor = new GtpyScriptEditor(GtpyContextManager::GlobalContext,
                                    this);

    QTextOption defaultOps = m_editor->document()->defaultTextOption();
    defaultOps.setFlags(defaultOps.flags() | QTextOption::ShowTabsAndSpaces /*|
                        QTextOption::ShowLineAndParagraphSeparators*/);

    m_editor->document()->setDefaultTextOption(defaultOps);

    GtPyHighlighter* highlighter = new GtPyHighlighter(m_editor->document());

    Q_UNUSED(highlighter)

    layout->addWidget(m_editor);

    setLayout(layout);
}

QString
GtpyPythonPlotConfigDialog::pythonScript()
{
    if (m_editor == nullptr)
    {
        return QString();
    }

    return m_editor->script();
}

void
GtpyPythonPlotConfigDialog::setScript(const QString& str)
{
    if (m_editor == nullptr)
    {
        return;
    }

    m_editor->setPlainText(str);
}
