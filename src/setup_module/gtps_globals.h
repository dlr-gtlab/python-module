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

}

} // namespace gtps

#endif // GTPSGLOBALS_H
