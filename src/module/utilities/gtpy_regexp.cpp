/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_regexp.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 15.02.2023
 * Author: Marvin Noethen (AT-TW)
 */

#include "gtpy_regexp.h"

QRegExp
gtpy::re::exceptLettersAndNumbers()
{
    return QRegExp{"[^A-Za-z0-9]+"};
}

