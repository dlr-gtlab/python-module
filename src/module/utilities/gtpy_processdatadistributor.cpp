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
#include "gtpy_taskapi.h"

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

    auto taskSpec = gtpy::parseTaskSpec(name);
    GtTask* task = getTask(data, taskSpec);

    if (!task)
    {
        return task;
    }

    task = qobject_cast<GtTask*>(task->clone());

    task->moveToThread(m_pythonTask->thread());

    // We need to run setParent from the main thread, where the python task is living
    // Otherwise qt will cause problems
    QMetaObject::invokeMethod(m_pythonTask, [this, task](){
        task->setParent(m_pythonTask);
    }, Qt::BlockingQueuedConnection);

    return task;
}
