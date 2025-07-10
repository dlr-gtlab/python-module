/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_sys.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.07.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_UTILS_H
#define GTPY_UTILS_H

#include <QString>

class GtProject;
class GtSession;

/**
 * Namespace for utility functions related to Python environment management.
 */
namespace gtpy
{

namespace utils
{

/**
 * @brief Adds a directory path to Python's sys.path if not already present.
 * @param path Directory path to add to sys.path.
 */
void addToSysPath(const QString& path);

/**
 * @brief Removes all occurrences of the specified directory path from
 * Python's sys.path.
 * @param path Directory path to remove from sys.path.
 */
void removeFromSysPath(const QString& path);

/**
 * @brief Returns the absolute path to the python modules directory inside the
 * given project's root directory.
 * @param project Pointer to the GtProject instance.
 * @return Absolute path to the python_modules directory.
 */
QString projectPyModulesPath(const GtProject* project);


} // namespace utils

} // namespace gtpy

#endif // GTPY_UTILS_H
