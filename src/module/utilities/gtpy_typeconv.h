/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_typeconv.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 *
 * Created on: 27.02.2026
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_TYPECONV_H
#define GTPY_TYPECONV_H

#include <QString>

namespace gtpy
{

namespace typeconv
{

/**
 * @brief Registers converters between C++ Qt metatypes and Python types.
 * This installs converter pairs for commonly used Qt types.
 */
void registerBuiltinConverters();

/**
 * @brief Maps a C++ source Qt metatype to a target Qt metatype used
 * for Python value conversion.
 *
 * @param sourceTypeName Qt metatype name to convert from.
 * @param targetTypeName Qt metatype name to convert to.
 * @return true if both type names were resolved and the mapping was
 * registered. false if any type name is unknown to QMetaType.
 */
bool mapMetaTypeToPythonTargetType(
    const QString& sourceTypeName, const QString& targetTypeName);

} // namespace typeconv

} // namespace gtpy

#endif // GTPY_TYPECONV_H
