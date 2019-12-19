/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_task.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_application.h"
#include "gt_objectpathproperty.h"
#include "gt_package.h"
#include "gt_project.h"
#include "gt_processdata.h"

#include "gtpy_contextmanager.h"

#include "gtpy_task.h"

GtpyTask::GtpyTask():
    m_script("script", "Skript"),
    m_calcDefinitions("calcDefinitions", "Calculator Definitions"),
    m_pyThreadId(-1)
{
    setObjectName("Python Task");

    registerProperty(m_script);
    registerProperty(m_calcDefinitions);

    m_script.hide();
    m_calcDefinitions.hide();

    foreach (const QString& modId, getModuleIds())
    {
        QString modClass = gtApp->modulePackageId(modId);

        if (!modClass.isEmpty())
        {
            GtObjectPathProperty* pathProp =
                    new GtObjectPathProperty(QStringLiteral(""),
                                             QStringLiteral(""),
                                             QStringLiteral(""),
                                             modId,
                                             this,
                                             QStringList() << modClass);
            pathProp->hide(true);
            registerProperty(*pathProp);

            m_dynamicPathProps << pathProp;
        }
    }

    connect(this, SIGNAL(stateChanged(GtProcessComponent::STATE)), this,
            SLOT(onStateChanged(GtProcessComponent::STATE)));
}

bool
GtpyTask::runIteration()
{
    GtpyContextManager::Context type = GtpyContextManager::TaskRunContext;

    GtpyContextManager::instance()->resetContext(type);

    GtpyContextManager::instance()->addTaskValue(type, this);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addObject(type,
                                         package->objectName(), package);
        }
    }

    gtInfo() << "running script...";

    m_pyThreadId = GtpyContextManager::instance()->currentPyThreadId();

    qDebug() << "id = " << m_pyThreadId;

    bool success;

    success = GtpyContextManager::instance()->evalScriptInterruptible(type, script(), true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(type,
                        package->objectName());
        }
    }

    gtInfo() << "...done!";

    return success;
}

QString
GtpyTask::script() const
{
    return m_script;
}

void
GtpyTask::setScript(QString script)
{
    qDebug() << script;
    script.replace("\n", "\r");

    m_script = script;

    qDebug() << "script saved!!!";
    qDebug() << m_script;
}

QStringList
GtpyTask::packageNames()
{
    QStringList list;

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        list.append(pathProp->path().toString());
    }

    return list;
}

bool
GtpyTask::childAccepted(GtObject* /*child*/)
{
    if (qobject_cast<GtProcessData*>(this->parent()))
    {
        return false;
    }

    return true;
}

QStringList
GtpyTask::getModuleIds()
{
    GtProject* project = gtApp->currentProject();

    if (project == Q_NULLPTR)
    {
        return QStringList();
    }

    return project->moduleIds();
}

void
GtpyTask::onStateChanged(STATE state)
{
    qDebug() << "STATE = " << this->currentState();

    if (m_pyThreadId < 0)
    {
        return;
    }

    if (state == GtProcessComponent::TERMINATION_REQUESTED)
    {
        qDebug() << "interrupt " << m_pyThreadId;
//        qDebug() << "current thread == " << GtpyContextManager::instance()->currentPyThreadId();
        GtpyContextManager::instance()->interruptPyThread(m_pyThreadId);
    }
}

QString
GtpyTask::calcDefinitions() const
{
    return m_calcDefinitions;
}

void
GtpyTask::setCalcDefinitions(QString& calcDefinitions)
{
    calcDefinitions.replace("\n", "\r");

    m_calcDefinitions = calcDefinitions;
}
