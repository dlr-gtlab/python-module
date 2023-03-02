/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_utilities.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.02.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_object.h"

#include "gtpy_regexp.h"
#include "gtpy_contextmanager.h"

#include "gtpy_utilities.h"

QString
gtpy::normalizeObjectName(QString objName)
{
    return objName.remove(gtpy::re::exceptLettersAndNumbers());
}

void
gtpy::gtObjectToPython(int contextId, GtObject* obj, const QString& varName)
{
    if (obj)
    {
        auto name = varName.isEmpty() ?
                    gtpy::normalizeObjectName(obj->objectName()) : varName;
        GtpyContextManager::instance()->addGtObject(contextId, name, obj);
    }
}

void
gtpy::removeObjectFromPython(int contextId, const QString& varName)
{
    GtpyContextManager::instance()->removeObject(contextId, varName);
}

void
gtpy::removeGtObjectFromPython(int contextId, GtObject* obj)
{
    if (obj)
    {
        auto varName = normalizeObjectName(obj->objectName());
        gtpy::removeObjectFromPython(contextId, varName);
    }
}
