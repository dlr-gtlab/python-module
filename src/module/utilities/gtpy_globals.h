/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_globals.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 11.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYGLOBALS_H
#define GTPYGLOBALS_H

#include <QString>

namespace GtpyGlobals
{
    ///Python Modules
    extern const QString GTOBJECT_WRAPPER_MODULE;
    extern const QString MODULE_GtCalculators;
    extern const QString MODULE_GtLogging;

    ///Python Classes
    extern const QString GTOBJECT_WRAPPER;
}
#endif // GTPYGLOBALS_H
