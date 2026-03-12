/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 */

#include "gtpy_taskapi.h"

#include <gt_application.h>
#include <gt_project.h>

#include <QThread>

namespace gtpy
{

TaskSpec
parseTaskSpec(const QString& spec)
{
    TaskSpec out;
    const int slash = spec.indexOf('/');

    if (slash < 0)
    {
        out.taskId = spec;

        return out;
    }

    out.hasExplicitGroup = true;
    out.groupId = spec.left(slash);
    out.taskId = spec.mid(slash + 1);

    return out;
}

/**
 * @brief Tries to initialize a task group
 * @param group
 * @param scope
 * @return
 */
bool
initTaskGroup(GtTaskGroup* group, GtTaskGroup::SCOPE scope)
{
    if (!group) return false;

    if (group->isInitialized()) return true;

    if (!gtApp || !gtApp->currentProject() || !group) return false;

    auto initGroupImpl  = [group, scope]() {
        group->read(gtApp->currentProject()->path(), scope);
    };

    // task group intialization must be done in the main thread
    if (QThread::currentThread() == gtApp->thread())
    {
        initGroupImpl();
    }
    else
    {
        QMetaObject::invokeMethod(gtApp, initGroupImpl,
                                  Qt::BlockingQueuedConnection);
    }

    if (!group->isInitialized())
    {
        gtError() << QObject::tr("Error initializing task group '%1'")
            .arg(group->objectName());
        return false;
    }

    return true;
}

#if GT_VERSION >= 0x020000
GtTaskGroup*
getTaskGroup(GtProcessData* data, TaskSpec spec)
{
    if  (!data) return nullptr;

    GtTaskGroup* group = nullptr;

    auto& groupName = spec.groupId;

    if (groupName.isEmpty()) return data->taskGroup();

    GtTaskGroup::SCOPE scope = GtTaskGroup::UNDEFINED;

    if (data->customGroupIds().contains(groupName))
    {
        scope = GtTaskGroup::CUSTOM;
    }
    else if(data->userGroupIds().contains(groupName))
    {
        scope = GtTaskGroup::USER;
    }

    if (scope == GtTaskGroup::UNDEFINED)
    {
        return nullptr;
    }

    auto* scopeElement = data->findDirectChild<GtObjectGroup*>(
        GtTaskGroup::scopeId(scope));

    if (!scopeElement)
    {
        return nullptr;
    }

    group = scopeElement->findDirectChild<GtTaskGroup*>(groupName);

    if (!group)
    {
        return nullptr;
    }

    if (!initTaskGroup(group, scope)) return nullptr;

    return group;
}
#endif

GtTask*
getTask(GtProcessData* data, TaskSpec spec)
{
    if (!data) return nullptr;

#if GT_VERSION >= 0x020000
    GtTaskGroup* group = getTaskGroup(data, spec);

    if (!group)
    {
        return nullptr;
    }

    return group->findDirectChild<GtTask*>(spec.taskId);
#else
    return data->findDirectChild<GtTask*>(spec.taskId);
#endif
}

} // namespace gtpy
