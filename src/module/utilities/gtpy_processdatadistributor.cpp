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

#include "gtpy_processdatadistributor.h"

GtpyProcessDataDistributor::GtpyProcessDataDistributor(GtTask* pythonTask)
{
    m_pythonTask = pythonTask;
}

GtTask*
GtpyProcessDataDistributor::taskElement(const QString& name)
{
    GtProcessData* data = gtApp->currentProject()->processData();

    if (!data)
    {
        return nullptr;
    }

#if GT_VERSION >= 0x020000
    GtTaskGroup* group = data->taskGroup();

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
        return task;
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
