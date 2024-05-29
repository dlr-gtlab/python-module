/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_transfer.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 15.02.2023
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYUTILITIES_H
#define GTPYUTILITIES_H

#include <QString>

#include "gt_globals.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_propertystructcontainer.h"
#endif

class GtObject;

/**
 * Namespace for functions that transfer objects to and from Python.
 */
namespace gtpy
{
namespace transfer
{
/**
 * @brief Adds the given GtObject to the Python context with the given id. If
 * varName is not empty, it will be the name of the object in Python.
 * Otherwise the objectname will be used as the name of the object in Python.
 * @param contextId Python context identifier.
 * @param obj GtObject to be added to the Python context.
 * @param varName Name of the object in Python.
 */
void gtObjectToPython(
        int contextId, GtObject* obj, const QString& varName = {});

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

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
/**
 * @brief Creates a dict containing the values of the property struct container.
 * @param contextId Id of the Python context to add the dict.
 * @param container Property struct container.
 */
void propStructToPython(
        int contextId, const GtPropertyStructContainer& container);

/**
 * @brief Takes the output dict from Python and puts its values into the
 * property struct container.
 * @param contextId Id of the Python context from which the dict is taken.
 * @param container Property struct container.
 */
void propStructFromPython(
        int contextId, GtPropertyStructContainer& container);

#endif

} // namespace transfer

} // namespace gtpy

#endif // GTPYUTILITIES_H
