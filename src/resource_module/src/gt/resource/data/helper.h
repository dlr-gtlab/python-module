/* GTlab - Gas Turbine laboratory
 * Source File: helper.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef HELPER_H
#define HELPER_H

#include <gt_project.h>
#include <gt_datamodel.h>

namespace gt
{

namespace resource
{

namespace data
{

namespace helper
{

inline GtProject* findProject(GtObject* obj)
{
    if (!obj) return {};
    return qobject_cast<GtProject*>(obj->findParent<GtProject*>());
}

inline GtProject* findProject(const QString& uuid)
{
    return findProject(gtDataModel->objectByUuid(uuid));
}

} // namespace helper

} // namespace data

} // namespace resource

} // namespace gt

#endif // HELPER_H
