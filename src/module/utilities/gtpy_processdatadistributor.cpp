/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_processdatadistributor.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 29.01.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_application.h"
#include "gt_processdata.h"
#include "gt_project.h"
#include "gtpy_task.h"
#include "gtpy_scriptcalculator.h"

#include "gtpy_processdatadistributor.h"

GtpyProcessDataDistributor::GtpyProcessDataDistributor(GtpyTask* pythonTask)
{
    m_pythonTask = pythonTask;
}

GtTask*
GtpyProcessDataDistributor::taskElement(const QString& name)
{
    GtProcessData* data = gtApp->currentProject()->processData();

    if (!data)
    {
        return Q_NULLPTR;
    }

    GtTask* task = data->findDirectChild<GtTask*>(name);

    if (!task)
    {
        return task;
    }

    QList<GtTask*> children = m_pythonTask->findDirectChildren<GtTask*>();

    foreach (GtTask* child, children)
    {
        if(child->uuid() == task->uuid())
        {
            delete child;
            child = Q_NULLPTR;
            break;
        }
    }

    task = qobject_cast<GtTask*>(task->clone());

    task->moveToThread(m_pythonTask->thread());

    task->setParent(m_pythonTask);

    return task;
}
