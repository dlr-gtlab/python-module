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
        enableSaveButton(false);
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
GtpyScriptCalculatorWizardPage::onSaveButtonClicked()
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
