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

#include <QRegExp>

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
QRegExp exceptLettersAndNumbers();

} // namespace re

} // namespace gtpy

#endif // GTPYREGEXP_H
