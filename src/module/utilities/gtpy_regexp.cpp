/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_regexp.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 15.02.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_regexp.h"

QRegExp
gtpy::re::exceptLettersAndNumbers()
{
    return QRegExp{"[^A-Za-z0-9]+"};
}


QRegularExpression
gtpy::re::forPythonIdentifiers()
{
    return QRegularExpression{"^[a-zA-Z_][a-zA-Z0-9_]*$"};
}

bool
gtpy::re::validation::isValidPythonIdentifier(const QString& ident)
{
    return ident.contains(forPythonIdentifiers());
}
