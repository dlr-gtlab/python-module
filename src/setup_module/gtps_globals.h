/* GTlab - Gas Turbine laboratory
 * Source File: gtps_globals.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.11.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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

} // namespace gtps

#endif // GTPSGLOBALS_H
