/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_transfer.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 15.02.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_transfer.h"

#include "gt_object.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_structproperty.h"
#endif

#include "gtpy_regexp.h"
#include "gtpy_contextmanager.h"

namespace {

QString
normalizeObjectName(QString objName)
{
    return objName.remove(gtpy::re::exceptLettersAndNumbers());
}

}

void
gtpy::transfer::gtObjectToPython(int contextId, GtObject* obj, const QString& varName)
{
    if (obj)
    {
        auto name = varName.isEmpty() ?
                    normalizeObjectName(obj->objectName()) : varName;
        GtpyContextManager::instance()->addGtObject(contextId, name, obj);
    }
}

void
gtpy::transfer::removeObjectFromPython(int contextId, const QString& varName)
{
    GtpyContextManager::instance()->removeObject(contextId, varName);
}

void
gtpy::transfer::removeGtObjectFromPython(int contextId, GtObject* obj)
{
    if (obj)
    {
        auto varName = normalizeObjectName(obj->objectName());
        removeObjectFromPython(contextId, varName);
    }
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
void
gtpy::transfer::propStructToPython(
        int contextId, const GtPropertyStructContainer& container)
{
    QVariantMap dict;

    for (const auto& instance : container)
    {
#if GT_VERSION >= GT_VERSION_CHECK(2, 1, 0)
        auto name = instance.ident();
#else
        auto name = instance.getMemberVal<QString>("name");
#endif

        if (!name.isEmpty())
        {
            bool ok{true};
            auto val = instance.getMemberValToVariant("value", &ok);

            if (ok)
            {
                dict.insert(name, val);
            }
        }
    }

    GtpyContextManager::instance()->addVariable(
                contextId, container.ident(), dict);
}

void
gtpy::transfer::propStructFromPython(
        int contextId, GtPropertyStructContainer& container)
{
    QVariantMap dict = GtpyContextManager::instance()->getVariable(
                contextId, container.ident()).toMap();

    for (auto& entry : container)
    {
#if GT_VERSION >= GT_VERSION_CHECK(2, 1, 0)
        auto argName = entry.ident();
#else
        auto argName = entry.getMemberVal<QString>("name");
#endif
        auto iter = dict.find(argName);
        if (iter != dict.end())
        {
            auto val = iter.value();
            entry.setMemberVal("value", val);
        }
    }
}
#endif
