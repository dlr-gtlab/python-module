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
    m_calcDefinitions("calcDefinitions", "Calculator Definitions")
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
}

bool
GtpyTask::runIteration()
{
    GtpyContextManager::instance()->addTaskValue(
                GtpyContextManager::TaskRunContext, this);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addObject(
                        GtpyContextManager::TaskRunContext,
                        package->objectName(), package);
        }
    }

    gtInfo() << "running script...";

    bool success;

    success = GtpyContextManager::instance()->evalScript(
                  GtpyContextManager::TaskRunContext,
                  script(), true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(
                        GtpyContextManager::TaskRunContext,
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
GtpyTask::setScript(QString& script)
{
    script.replace("\n", "\r");

    m_script = script;
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
