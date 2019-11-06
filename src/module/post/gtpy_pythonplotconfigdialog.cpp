/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotconfigdialog.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
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
    if (m_editor == Q_NULLPTR)
    {
        return QString();
    }

    return m_editor->script();
}

void
GtpyPythonPlotConfigDialog::setScript(const QString& str)
{
    if (m_editor == Q_NULLPTR)
    {
        return;
    }

    m_editor->setPlainText(str);
}
