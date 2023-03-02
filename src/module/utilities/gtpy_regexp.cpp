/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_regexp.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.02.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_regexp.h"

QRegExp
gtpy::re::exceptLettersAndNumbers()
{
    return QRegExp{"[^A-Za-z0-9]+"};
}

