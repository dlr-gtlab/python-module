/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_processdatadistributor.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 29.01.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_application.h"
#include "gt_processdata.h"
#include "gt_project.h"
#include "gt_task.h"
#include "gt_logging.h"

#include "gtpy_processdatadistributor.h"

GtpyProcessDataDistributor::GtpyProcessDataDistributor(GtTask* pythonTask)
{
    m_pythonTask = pythonTask;
}

GtTask*
GtpyProcessDataDistributor::taskElement(const QString& name,
                                        const QString groupName)
{
    GtProcessData* data = gtApp->currentProject()->processData();

    if (!data)
    {
        return nullptr;
    }

#if GT_VERSION >= 0x020000
    GtTaskGroup* group = nullptr;

    if (groupName.isEmpty())
    {
        data->taskGroup();
    }
    else
    {
        GtTaskGroup::SCOPE scope = GtTaskGroup::UNDEFINED;
        QString scopeKey = "";

        if (!data->initAllTaskGroups(gtApp->currentProject()->path()))
        {
            return nullptr;
        }

        if (data->customGroupIds().contains(groupName))
        {
            scope = GtTaskGroup::CUSTOM;
            scopeKey = "_custom";
        }
        else if(data->userGroupIds().contains(groupName))
        {
            scope = GtTaskGroup::USER;
            scopeKey = "_user";
        }

        if (scope == GtTaskGroup::UNDEFINED)
        {
            return nullptr;
        }

        auto* scopeElement = data->findDirectChild<GtObjectGroup*>(scopeKey);

        if (!scopeElement)
        {
            return nullptr;
        }

        group = scopeElement->findDirectChild<GtTaskGroup*>(groupName);

        if (!group)
        {
            return nullptr;
        }

        if (!group->isInitialized())
        {
            gtDebug() << QObject::tr("Usage of uninitialized task "
                                     "group %1").arg(groupName);
            return nullptr;
        }
    }

    if (!group)
    {
        return nullptr;
    }

    GtTask* task = group->findDirectChild<GtTask*>(name);
#else
    GtTask* task = data->findDirectChild<GtTask*>(name);
#endif
    if (!task)
    {
        return nullptr;
    }

    //    QList<GtTask*> children = m_pythonTask->findDirectChildren<GtTask*>();

    //    foreach (GtTask* child, children)
    //    {
    //        if(child->uuid() == task->uuid())
    //        {
    //            delete child;
    //            child = nullptr;
    //            break;
    //        }
    //    }

    task = qobject_cast<GtTask*>(task->clone());

    task->moveToThread(m_pythonTask->thread());

    // We need to run setParent from the main thread, where the python task is living
    // Otherwise qt will cause problems
    QMetaObject::invokeMethod(m_pythonTask, [this, task](){
        task->setParent(m_pythonTask);
    }, Qt::BlockingQueuedConnection);

    return task;
}
