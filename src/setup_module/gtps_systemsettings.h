/* GTlab - Gas Turbine laboratory
 * Source File: gtps_systemsettings.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 18.01.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPSSYSTEMSETTINGS_H
#define GTPSSYSTEMSETTINGS_H

#include <QString>

namespace gtps {

namespace system {

/**
 * @brief Returns the system specific separator used to separate system
 * paths in the environment variables.
 * @return The system specific separator used to separate system paths in the
 * environment variables.
 */
QString pathSeperator();

/**
 * @brief It clears the PYTHONPATH and the PYTHONHOME variable.
 */
void clearPythonVars();

/**
 * @brief Returns the value of the PYTHONPATH environment variable.
 * @return The value of the PYTHONPATH environment variable.
 */
QByteArray pythonPath();

/**
 * @brief Sets the value of the PYTHONPATH environment variable to the
 * given value.
 * @param val Value to be stored in the PYTHONPATH environment variable.
 */
void setPythonPath(const QByteArray& val);

/**
 * @brief Returns the value of the PYTHONHOME environment variable.
 * @return The value of the PYTHONHOME environment variable.
 */
QByteArray pythonHome();

/**
 * @brief Sets the value of the PYTHONHOME environment variable to the
 * given value.
 * @param val Value to be stored in the PYTHONHOME environment variable.
 */
void setPythonHome(const QByteArray& val);

} // namespace system

} // namespace gtps

#endif // GTPSSYSTEMSETTINGS_H
