/* GTlab - Gas Turbine laboratory
 * Source File: gtps_globals.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 30.11.2022
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPSGLOBALS_H
#define GTPSGLOBALS_H

#include "gt_versionnumber.h"

#include <QVariant>

namespace gtps {

/**
 * @brief Takes a version number and returns a string containing the major and
 * minor version separated by a dot.
 * @param version Version number.
 * @return A string containing the major and minor version separated by a dot.
 */
QString apiVersionStr(const GtVersionNumber& version);

namespace python {

namespace module {

/**
 * @brief Returns the Python module id depending on the given version number.
 * @param version Version number.
 * @return Python module id depending on the given version number.
 */
QString pythonModuleId(const GtVersionNumber& version);

} // namespace mdoule

namespace version
{

/**
 * @brief Returns a vector of the supported Python versions.
 * @return Vector of supported Python version.
 */
QVector<GtVersionNumber> supportedVersions();

/**
 * @brief Checks whether the given Python version is supported.
 * @param version Python version.
 * @return True if the Python version is supported, otherwise false.
 */
bool isSupported(const GtVersionNumber& version);

} // namespace version

} // namespace python

namespace settings
{

/**
 * @brief Returns the setting path.
 * @param settingId Id of the setting.
 * @return The setting path.
 */
QString path(const QString& settingId);

/**
 * @brief Saves pre-registered python settings
 */
void setSetting(const QString& settingId, const QVariant& value);

/**
 * @brief Queries pre-registeted python settings
 */
QVariant getSetting(const QString& settingId);

/**
 * @brief Returns the path of custom python interpreter, stored in settings
 * @return
 */
QString customPythonPath();

/**
 * @brief Checks, whether the embedded interpreter should be used
 * @return
 */
bool useEmbeddedPython();

}

/**
 * @brief Return the path of the embedded python executable
 *
 * If no embedded python is available, an empty string is returned
 */
QString embeddedPythonPath();

/**
 * @brief Returns the path of the python interpreter
 */
QString pythonPath();


} // namespace gtps

#endif // GTPSGLOBALS_H
