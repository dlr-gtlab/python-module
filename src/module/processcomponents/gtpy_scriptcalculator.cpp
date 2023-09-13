/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculator.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_package.h"
#include "gt_objectpathproperty.h"
#include "gt_processdata.h"

#include "gtpy_contextmanager.h"
#include "gtpy_packageiteration.h"
#include "gtpy_wizardgeometries.h"

#include "gtpy_scriptcalculator.h"


GtpyScriptCalculator::GtpyScriptCalculator()
{
    setObjectName("Python Script Editor");


#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    registerPropertyStructContainer(m_inputArgs);
    registerMonitoringPropertyStructContainer(m_outputArgs);
#endif

    registerProperty(m_script);
    registerProperty(m_replaceTabBySpaces);
    registerProperty(m_tabSize);

    using PackageInfo = gtpy::package::PackageInfo;
    gtpy::package::forEachPackage([&](const PackageInfo& pInfo){
        auto pathProp = new GtObjectPathProperty(
                    QStringLiteral(""), QStringLiteral(""), QStringLiteral(""),
                    pInfo.modId, this, QStringList() << pInfo.className);
        pathProp->hide(true);
        registerProperty(*pathProp);

        m_dynamicPathProps << pathProp; });

    connect(this, SIGNAL(stateChanged(GtProcessComponent::STATE)), this,
            SLOT(onStateChanged(GtProcessComponent::STATE)));

    connect(this, SIGNAL(deletedFromDatamodel(QString)),
            GtpyWizardGeometries::instance(),
            SLOT(processElementDeleted(QString)));
}

GtpyScriptCalculator::~GtpyScriptCalculator()
{
    if (this->findParent<GtProcessData*>())
    {
        emit deletedFromDatamodel(this->uuid());
    }
}

bool
GtpyScriptCalculator::run()
{
    int contextId = GtpyContextManager::instance()->createNewContext(
        GtpyContextManager::CalculatorRunContext, true);

    return evalScript(contextId);
}

void
GtpyScriptCalculator::connectWithRootTask(bool connection)
{
    if (auto* root = qobject_cast<GtProcessComponent*>(findRoot<GtObject*>()))
    {
        if (connection)
        {
            connect(root, SIGNAL(stateChanged(GtProcessComponent::STATE)),
                    this, SLOT(onTaskStateChanged(GtProcessComponent::STATE)));
        }
        else
        {
            disconnect(root, SIGNAL(stateChanged(GtProcessComponent::STATE)),
                       this, SLOT(onTaskStateChanged(GtProcessComponent::STATE)));
        }
    }
}

GtPackage*
GtpyScriptCalculator::dataPackage(const GtObjectPath& pkgPath)
{
    return data<GtPackage*>(pkgPath);
}

void
GtpyScriptCalculator::onStateChanged(GtProcessComponent::STATE state)
{
    connectWithRootTask(state == GtProcessComponent::RUNNING ? true : false);
}

void
GtpyScriptCalculator::onTaskStateChanged(GtProcessComponent::STATE state) const
{
    if (m_pyThreadId >= 0 && state == GtProcessComponent::TERMINATION_REQUESTED)
    {
        GtpyContextManager::instance()->interruptPyThread(m_pyThreadId);
    }
}
