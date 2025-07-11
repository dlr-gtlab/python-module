/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_task.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_package.h"
#include "gt_objectpathproperty.h"
#include "gt_processdata.h"

#include "gtpy_contextmanager.h"
#include "gtpy_wizardgeometries.h"

#include "gtpy_task.h"

GtpyTask::GtpyTask()
{
    setObjectName("Python Task");

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    registerPropertyStructContainer(m_inputArgs);
    registerMonitoringPropertyStructContainer(m_outputArgs);
#endif

    registerProperty(m_script);
    registerProperty(m_replaceTabBySpaces);
    registerProperty(m_tabSize);

    for (auto* pathProp : qAsConst(m_dynamicPathProps))
    {
        registerProperty(*pathProp);
    }

    connect(this, SIGNAL(stateChanged(GtProcessComponent::STATE)), this,
            SLOT(onStateChanged(GtProcessComponent::STATE)));

    connect(this, SIGNAL(deletedFromDatamodel(QString)),
            GtpyWizardGeometries::instance(),
            SLOT(processElementDeleted(QString)));
}

GtpyTask::~GtpyTask()
{
    if (this->findParent<GtProcessData*>())
    {
        emit deletedFromDatamodel(this->uuid());
    }
}

bool
GtpyTask::runIteration()
{
    int contextId = GtpyContextManager::instance()->createNewContext(
        GtpyContextManager::TaskRunContext, true);

    GtpyContextManager::instance()->setLoggingPrefix(contextId, objectName());

    ///Initialize context with data model objects
    GtpyContextManager::instance()->addTaskValue(contextId, this);
    auto success = evalScript(contextId);

    emit transferMonitoringProperties();

    return success;
}

bool
GtpyTask::childAccepted(GtObject* /*child*/)
{
#if GT_VERSION >= 0x020000
    return qobject_cast<GtTaskGroup*>(this->parent()) == nullptr;
#else
    return qobject_cast<GtProcessData*>(this->parent()) == nullptr;
#endif
}

GtPackage*
GtpyTask::dataPackage(const GtObjectPath& pkgPath)
{
    return data<GtPackage*>(pkgPath);
}

void
GtpyTask::onStateChanged(STATE state) const
{
    if (m_pyThreadId >= 0 && state == GtProcessComponent::TERMINATION_REQUESTED)
    {
        GtpyContextManager::instance()->interruptPyThread(m_pyThreadId);
    }
}

