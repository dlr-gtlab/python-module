/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 */

#ifndef GTPY_TASKAPI_H
#define GTPY_TASKAPI_H

#include <gt_task.h>
#include <gt_processdata.h>

#include <QString>

namespace gtpy
{

/**
 * @brief Parsed task specification in `taskId` or `groupId/taskId` form.
 */
struct TaskSpec
{
    QString groupId;
    QString taskId;
    bool hasExplicitGroup{false};
};

/**
 * @brief Parse a task specification string.
 *
 * @param spec Task identifier in `taskId` or `groupId/taskId` form.
 * @return Parsed task specification parts.
 */
TaskSpec parseTaskSpec(const QString& spec);

/**
 * @brief Gets the task from the process
 *
 * @param data The processdata owning the task.
 * @param taskSpec Task spec in `taskId` or `groupId/taskId` form.
 */
GtTask* getTask(GtProcessData* data, TaskSpec spec);

} // namespace gtpy

#endif // GTPY_TASKAPI_H
