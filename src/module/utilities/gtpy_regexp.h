/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_regexp.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.02.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
