/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculatorwizardpage.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */


// GTlab framework includes
#include "gt_datamodel.h"
#include "gt_command.h"
#include "gt_application.h"
#include "gt_project.h"
#include "gt_processfactory.h"
#include "gt_objectmemento.h"
#include "gt_abstractprocessprovider.h"

#include "gtpy_transfer.h"

#include "gtpy_scriptcalculatorwizardpage.h"

GtpyScriptCalculatorWizardPage::GtpyScriptCalculatorWizardPage() :
    GtpyAbstractScriptingWizardPage(GtpyContextManager::ScriptEditorContext),
    m_calc{nullptr}
{

}

void
GtpyScriptCalculatorWizardPage::initialization()
{
    GtObjectMemento memento = provider()->componentData();

    if (memento.isNull())
    {
        return;
    }

    if (m_calc)
    {
        delete m_calc;
    }

    m_calc = memento.restore<GtpyScriptCalculator*>(gtProcessFactory);

    if (!m_calc)
    {
        return;
    }

    m_calc->setParent(this);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    gtpy::transfer::propStructToPython(m_contextId, m_calc->inputArgs());
    gtpy::transfer::propStructToPython(m_contextId, m_calc->outputArgs());
#endif

    if (!gtDataModel->objectByUuid(m_calc->uuid()))
    {
        enableSaving(false);
    }

    setPlainTextToEditor(m_calc->script());
}

bool
GtpyScriptCalculatorWizardPage::validation()
{
    if (!m_calc)
    {
        return false;
    }

    m_calc->setScript(editorText());
    provider()->setComponentData(m_calc->toMemento());

    return true;
}

void
GtpyScriptCalculatorWizardPage::saveScript()
{
    if (!m_calc)
    {
        return;
    }

    m_calc->setScript(editorText());

    GtObjectMemento memento = m_calc->toMemento();

    if (memento.isNull())
    {
        return;
    }

    auto* obj = gtDataModel->objectByUuid(m_calc->uuid());

    if (auto* calc = qobject_cast<GtCalculator*>(obj))
    {
        GtCommand command =
            gtApp->startCommand(gtApp->currentProject(),
                                calc->objectName() +
                                tr(" configuration changed"));
        calc->fromMemento(memento);
        gtApp->endCommand(command);
    }
}

QString
GtpyScriptCalculatorWizardPage::componentUuid() const
{
    return m_calc ? m_calc->uuid() : QString();
}

void
GtpyScriptCalculatorWizardPage::setComponentName(const QString& name)
{
    if (m_calc)
    {
        m_calc->setObjectName(name);
    }
}

GtpyEditorSettings*
GtpyScriptCalculatorWizardPage::createSettings()
{
    if (!m_calc)
    {
        return nullptr;
    }

    auto* pref = new GtpyEditorSettings(this);

    if (m_calc->tabSize() <= 0)
    {
        pref->setTabSize(4);
        pref->setReplaceTabBySpace(false);
    }
    else
    {
        pref->setTabSize(m_calc->tabSize());
        pref->setReplaceTabBySpace(m_calc->replaceTabBySpaces());
    }

    return pref;
}

void
GtpyScriptCalculatorWizardPage::saveSettings(GtpyEditorSettings* pref)
{
    if (pref && m_calc)
    {
        m_calc->setTabSize(pref->tabSize());
        m_calc->setReplaceTabBySpaces(pref->replaceTabBySpace());
    }
}
