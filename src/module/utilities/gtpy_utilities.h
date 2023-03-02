/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_utilities.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.02.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYUTILITIES_H
#define GTPYUTILITIES_H

#include <QString>

class GtObject;

/**
 * Namespace for the utilities used in the GTlab Python module
 */
namespace gtpy {

/**
 * @brief Removes special characters from the given objName so it can be used
 * as a variable name in Python.
 * @param objName Object name to be normalized.
 * @return The normalized object name.
 */
QString normalizeObjectName(QString objName);

/**
 * @brief Adds the given GtObject to the Python context with the given id. If
 * varName is not empty, it will be the name of the object in Python.
 * Otherwise the objectname will be used as the name of the object in Python.
 * @param contextId Python context identifier.
 * @param obj GtObject to be added to the Python context.
 * @param varName Name of the object in Python.
 */
void gtObjectToPython(int contextId, GtObject* obj,
                      const QString& varName = {});

/**
 * @brief Removes the object with the given varName from the Python context
 * with the given contextId.
 * @param contextId Python context identifier.
 * @param varName Name of the object to be removed from the Python context.
 */
void removeObjectFromPython(int contextId, const QString& varName);

/**
 * @brief Removes the given GtObject from the Python context with the given id.
 * The function assumes that the objectname was used as the name for the
 * object in Python.
 * @param contextId Python context identifier.
 * @param obj GtObject to be removed from the Python context
 */
void removeGtObjectFromPython(int contextId,  GtObject* obj);

struct Package
{
    QString modId;
    QString className;
    QString objectName;
};

void forEachPackage();

} // namespace gtpy

#endif // GTPYUTILITIES_H
