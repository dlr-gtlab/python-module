/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculatorwizardpage.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */


// GTlab framework includes
#include "gt_datamodel.h"
#include "gt_command.h"
#include "gt_application.h"
#include "gt_project.h"
#include "gt_processfactory.h"
#include "gt_objectmemento.h"
#include "gt_abstractprocessprovider.h"

#include "gtpy_scriptcalculatorwizardpage.h"

GtpyScriptCalculatorWizardPage::GtpyScriptCalculatorWizardPage() :
    GtpyAbstractScriptingWizardPage(GtpyContextManager::ScriptEditorContext),
    m_calc(Q_NULLPTR)
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

    if (m_calc != Q_NULLPTR)
    {
        delete m_calc;
    }

    m_calc = memento.restore<GtpyScriptCalculator*>(gtProcessFactory);

    if (m_calc == Q_NULLPTR)
    {
        return;
    }

    m_calc->setParent(this);

    if (gtDataModel->objectByUuid(m_calc->uuid()) == Q_NULLPTR)
    {
        enableSaving(false);
    }

    setPlainTextToEditor(m_calc->script());

    setPackageNames(m_calc->packageNames());
}

bool
GtpyScriptCalculatorWizardPage::validation()
{
    if (m_calc == Q_NULLPTR)
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
    if (m_calc == Q_NULLPTR)
    {
        return;
    }

    m_calc->setScript(editorText());

    GtObjectMemento memento = m_calc->toMemento();

    if (memento.isNull())
    {
        return;
    }

    GtObject* obj = gtDataModel->objectByUuid(m_calc->uuid());

    if (GtCalculator* calc = qobject_cast<GtCalculator*>(obj))
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
    QString uuid;

    if (m_calc != Q_NULLPTR)
    {
        uuid = m_calc->uuid();
    }

    return uuid;
}

void
GtpyScriptCalculatorWizardPage::setComponentName(const QString& name)
{
    if (m_calc != Q_NULLPTR)
    {
        m_calc->setObjectName(name);
    }
}

GtpyEditorSettings*
GtpyScriptCalculatorWizardPage::createSettings()
{
    GtpyEditorSettings* pref = Q_NULLPTR;

    if (m_calc != Q_NULLPTR)
    {
        pref = new GtpyEditorSettings(this);

        if (m_calc->tabSize() <= 0)
        {
            pref->setTabSize(4);
        }
        else
        {
            pref->setTabSize(m_calc->tabSize());
        }

        pref->setReplaceTabBySpace(m_calc->replaceTabBySpaces());
    }

    return pref;
}

void
GtpyScriptCalculatorWizardPage::saveSettings(GtpyEditorSettings* pref)
{
    if (pref != Q_NULLPTR && m_calc != Q_NULLPTR)
    {
        m_calc->setTabSize(pref->tabSize());
        m_calc->setReplaceTabBySpaces(pref->replaceTabBySpace());
    }
}
