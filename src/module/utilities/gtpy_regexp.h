/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_regexp.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 15.02.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYREGEXP_H
#define GTPYREGEXP_H

#include <QRegularExpression>

/**
 * namespace for RegualrExpressions used in the GTlab Python Module
 */
namespace gtpy
{
namespace re
{

/**
 * @brief Everything except letters and numbers.
 * @return Returns a QRegExp instance.
 */
QRegularExpression exceptLettersAndNumbers();

/**
 * @brief Returns a regular expression that matches valid Python identifiers.
 * Identifiers in Python are used for variable names, function names and other
 * objects. A valid identifier must start with a letter or an underscore and
 * can contain letters, numbers and underscores.
 * @return A QRegularExpression that matches strings consisting of letters,
 * digits and underscores starting with a letter or underscore.
 */
QRegularExpression forPythonIdentifiers();

namespace validation
{

/**
 * @brief Checks if the given string is a valid Python identifier. Identifiers
 * in Python are used for variable names, function names and other objects.
 * A valid identifier must start with a letter or an underscore and can contain
 * letters, numbers and underscores.
 * @param ident The string to check.
 * @return True if the ident is a valid Python identifier, otherwise false.
 */
bool isValidPythonIdentifier(const QString& ident);

} // namespace validation

} // namespace re

} // namespace gtpy

#endif // GTPYREGEXP_H
